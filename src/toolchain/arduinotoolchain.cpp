/****************************************************************************
**
** Copyright (c) 2020 Thomas COIN
** Contact: Thomas Coin<esvcorp@gmail.com>
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in all
** copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
** SOFTWARE.
**
****************************************************************************/
#include "arduinotoolchain.h"
#include "../arduinosettings.h"
#include "../arduinoconstants.h"

#include <projectexplorer/toolchain.h>
#include <projectexplorer/projectexplorerconstants.h>

#include <utils/pathchooser.h>
#include <utils/environment.h>
#include <utils/algorithm.h>
#include <utils/qtcassert.h>
#include <utils/synchronousprocess.h>

#include <QLabel>
#include <QFormLayout>
#include <QDir>
#include <QtDebug>

using namespace ProjectExplorer;
using namespace Utils;

namespace Arduino {
namespace Internal {

// --------------------------------------------------------------------------
// Static functions
// --------------------------------------------------------------------------

static QStringList reinterpretOptions(const QStringList &args)
{
    return args;
}

static Utils::FileName findLocalCompiler(const Utils::FileName &compilerPath,
                                         const Environment &env)
{
    // Find the "real" compiler if icecc, distcc or similar are in use. Ignore ccache, since that
    // is local already.

    // Get the path to the compiler, ignoring direct calls to icecc and distcc as we can not
    // do anything about those.
    const Utils::FileName compilerDir = compilerPath.parentDir();
    const QString compilerDirString = compilerDir.toString();
    if (!compilerDirString.contains("icecc") && !compilerDirString.contains("distcc"))
        return compilerPath;

    QStringList pathComponents = env.path();
    auto it = std::find_if(pathComponents.begin(), pathComponents.end(),
                           [compilerDir](const QString &p) {
        return Utils::FileName::fromString(p) == compilerDir;
    });
    if (it != pathComponents.end()) {
        std::rotate(pathComponents.begin(), it, pathComponents.end());
        pathComponents.removeFirst(); // remove directory of compilerPath
                                      // No need to put it at the end again, it is in PATH anyway...
    }

    // This effectively searches the PATH twice, once via pathComponents and once via PATH itself:
    // searchInPath filters duplicates, so that will not hurt.
    const Utils::FileName path = env.searchInPath(compilerPath.fileName(), pathComponents);

    return path.isEmpty() ? compilerPath : path;
}

static QByteArray runGcc(const FileName &gcc, const QStringList &arguments, const QStringList &env)
{
    if (gcc.isEmpty() || !gcc.toFileInfo().isExecutable())
        return QByteArray();

    SynchronousProcess cpp;
    QStringList environment(env);
    Utils::Environment::setupEnglishOutput(&environment);

    cpp.setEnvironment(environment);
    cpp.setTimeoutS(10);
    SynchronousProcessResponse response =  cpp.runBlocking(gcc.toString(), arguments);
    if (response.result != SynchronousProcessResponse::Finished ||
            response.exitCode != 0) {
        qWarning() << response.exitMessage(gcc.toString(), 10);
        return QByteArray();
    }

    return response.allOutput().toUtf8();
}

static const QStringList languageOption(Core::Id languageId)
{
    if (languageId == ProjectExplorer::Constants::C_LANGUAGE_ID)
        return {"-x", "c"};
    return {"-x", "c++"};
}

static const QStringList gccPredefinedMacrosOptions(Core::Id languageId)
{
    return languageOption(languageId) + QStringList({"-E", "-dM"});
}

static ProjectExplorer::Macros gccPredefinedMacros(const FileName &gcc,
                                                   const QStringList &args,
                                                   const QStringList &env)
{
    QStringList arguments = args;
    arguments << "-";

    ProjectExplorer::Macros predefinedMacros = Macro::toMacros(runGcc(gcc, arguments, env));
    // Sanity check in case we get an error message instead of real output:
    QTC_CHECK(predefinedMacros.isEmpty()
              || predefinedMacros.front().type == ProjectExplorer::MacroType::Define);
    if (HostOsInfo::isMacHost()) {
        // Turn off flag indicating Apple's blocks support
        const ProjectExplorer::Macro blocksDefine("__BLOCKS__", "1");
        const ProjectExplorer::Macro blocksUndefine("__BLOCKS__", ProjectExplorer::MacroType::Undefine);
        const int idx = predefinedMacros.indexOf(blocksDefine);
        if (idx != -1)
            predefinedMacros[idx] = blocksUndefine;

        // Define __strong and __weak (used for Apple's GC extension of C) to be empty
        predefinedMacros.append({"__strong"});
        predefinedMacros.append({"__weak"});
    }
    return predefinedMacros;
}

static QList<Abi> guessGccAbi(const QString &m, const ProjectExplorer::Macros &macros)
{
    QList<Abi> abiList;

    Abi guessed = Abi::abiFromTargetTriplet(m);
    if (guessed.isNull())
        return abiList;

    Abi::Architecture arch = guessed.architecture();
    Abi::OS os = guessed.os();
    Abi::OSFlavor flavor = guessed.osFlavor();
    Abi::BinaryFormat format = guessed.binaryFormat();
    int width = guessed.wordWidth();

    const Macro sizeOfMacro = Utils::findOrDefault(macros, [](const Macro &m) { return m.key == "__SIZEOF_SIZE_T__"; });
    if (sizeOfMacro.isValid() && sizeOfMacro.type == MacroType::Define)
        width = sizeOfMacro.value.toInt() * 8;
    const Macro &mscVerMacro = Utils::findOrDefault(macros, [](const Macro &m) { return m.key == "_MSC_VER"; });
    if (mscVerMacro.type == MacroType::Define) {
        const int msvcVersion = mscVerMacro.value.toInt();
        flavor = Abi::flavorForMsvcVersion(msvcVersion);
    }

    if (os == Abi::DarwinOS) {
        // Apple does PPC and x86!
        abiList << Abi(arch, os, flavor, format, width);
        abiList << Abi(arch, os, flavor, format, width == 64 ? 32 : 64);
    } else if (arch == Abi::X86Architecture && (width == 0 || width == 64)) {
        abiList << Abi(arch, os, flavor, format, 64);
        abiList << Abi(arch, os, flavor, format, 32);
    } else {
        abiList << Abi(arch, os, flavor, format, width);
    }
    return abiList;
}


static GccToolChain::DetectedAbisResult guessGccAbi(const FileName &path, const QStringList &env,
                                                   const ProjectExplorer::Macros &macros,
                                                   const QStringList &extraArgs = QStringList())
{
    if (path.isEmpty())
        return GccToolChain::DetectedAbisResult();

    QStringList arguments = extraArgs;
    arguments << "-dumpmachine";
    QString machine = QString::fromLocal8Bit(runGcc(path, arguments, env)).trimmed();
    if (machine.isEmpty())
        return GccToolChain::DetectedAbisResult(); // no need to continue if running failed once...
    return GccToolChain::DetectedAbisResult(guessGccAbi(machine, macros), machine);
}

// --------------------------------------------------------------------------
// ArduinoToolChain
// --------------------------------------------------------------------------

ArduinoToolChain::ArduinoToolChain(ToolChain::Detection d)
    : GccToolChain(Constants::ARDUINO_TOOLCHAIN_ID, d)
{
    setOptionsReinterpreter(&reinterpretOptions);
}

ArduinoToolChain::ArduinoToolChain(Core::Id l, ToolChain::Detection d)
    : ArduinoToolChain(d)
{
    setLanguage(l);
}

QString ArduinoToolChain::typeDisplayName() const
{
    return ArduinoToolChainFactory::tr("AVR GCC");
}

ToolChainConfigWidget *ArduinoToolChain::configurationWidget()
{
    return new ArduinoToolChainConfigWidget(this);
}

// --------------------------------------------------------------------------
// ArduinoToolChainFactory
// --------------------------------------------------------------------------

ArduinoToolChainFactory::ArduinoToolChainFactory()
{
    setDisplayName(tr("AVR GCC"));
}

QSet<Core::Id> ArduinoToolChainFactory::supportedLanguages() const
{
    return
    {
        ProjectExplorer::Constants::C_LANGUAGE_ID,
        ProjectExplorer::Constants::CXX_LANGUAGE_ID
    };
}

QList<ToolChain *> ArduinoToolChainFactory::autoDetect(const QList<ToolChain *> &alreadyKnown)
{
    QList<ToolChain *> tcs;
    QList<ToolChain *> known = alreadyKnown;

    static const Abi avrAbi(Abi::AvrArchitecture, Abi::BareMetalOS, Abi::GenericBareMetalFlavor, Abi::ElfFormat, 16);

    tcs.append(autoDetectToolchains("avr-g++", avrAbi, ProjectExplorer::Constants::CXX_LANGUAGE_ID,
                                    Constants::ARDUINO_TOOLCHAIN_ID, alreadyKnown));
    tcs.append(autoDetectToolchains("avr-gcc", avrAbi, ProjectExplorer::Constants::C_LANGUAGE_ID,
                                    Constants::ARDUINO_TOOLCHAIN_ID, alreadyKnown));
    known.append(tcs);
    if (HostOsInfo::isLinuxHost()) {
        for (const QString &dir : QStringList({ "/usr/bin", "/usr/local/bin" })) {
            QDir binDir(dir);
            auto gccProbe = [&](const QString &name, Core::Id language) {
                for (const QString &entry : binDir.entryList( { name}, QDir::Files | QDir::Executable)) {
                    const QString fileName = FileName::fromString(entry).fileName();
                    if (fileName == "c89-gcc" || fileName == "c99-gcc")
                        continue;

                    const Abi abi = Abi();
                    tcs.append(autoDetectToolchains(entry, abi, language,
                                                    Constants::ARDUINO_TOOLCHAIN_ID, known));
                    known.append(tcs);
                }
            };
            gccProbe("avr-g++", ProjectExplorer::Constants::CXX_LANGUAGE_ID);
            gccProbe("avr-gcc", ProjectExplorer::Constants::C_LANGUAGE_ID);
        }
    }

    // Detect AVR compiler in Arduino SDK folder
    /// @todo Check if Arduino SDK settings are OK
    const FileName avrGcc = FileName(ArduinoSettings::instance()->avrBinPath()).appendPath("avr-gcc");
    const FileName avrGpp = FileName(ArduinoSettings::instance()->avrBinPath()).appendPath("avr-g++");
    tcs.append(autoDetectToolChain(avrGcc,
                                   ProjectExplorer::Constants::CXX_LANGUAGE_ID,
                                   avrAbi));

    tcs.append(autoDetectToolChain(avrGpp,
                                   ProjectExplorer::Constants::C_LANGUAGE_ID,
                                   avrAbi));

    return tcs;
}

QList<ToolChain *> ArduinoToolChainFactory::autoDetect(const FileName &compilerPath, const Core::Id &language)
{
    const QString fileName = compilerPath.fileName();

    if ((language == ProjectExplorer::Constants::C_LANGUAGE_ID && (fileName.startsWith("avr-gcc")))
            || (language == ProjectExplorer::Constants::CXX_LANGUAGE_ID && (fileName.startsWith("avr-g++"))))
        return autoDetectToolChain(compilerPath, language);
    return QList<ToolChain *>();
}

QList<ToolChain *> ArduinoToolChainFactory::autoDetectToolchains(const QString &compiler,
                                                                 const Abi &requiredAbi,
                                                                 Core::Id language,
                                                                 const Core::Id requiredTypeId,
                                                                 const QList<ToolChain *> &alreadyKnown)
{
    QList<ToolChain *> result;

    Environment systemEnvironment = Environment::systemEnvironment();
    const FileName compilerPath = systemEnvironment.searchInPath(compiler);
    if (compilerPath.isEmpty())
        return result;
    const FileName canonicalPath = FileUtils::canonicalPath(compilerPath);

    result = Utils::filtered(alreadyKnown, [requiredTypeId, compilerPath](ToolChain *tc) {
        return tc->typeId() == requiredTypeId && tc->compilerCommand() == compilerPath;
    });
    if (!result.isEmpty()) {
        for (ToolChain *tc : result) {
            if (tc->isAutoDetected())
                tc->setLanguage(language);
        }
        return result;
    }

    result = autoDetectToolChain(compilerPath, language, requiredAbi);

    if (!requiredAbi.isNull()) {
        const Abi alternateAbi = Abi(requiredAbi.architecture(), requiredAbi.os(),
                                     requiredAbi.osFlavor(), requiredAbi.binaryFormat(), 32);
        ToolChain *abiTc = Utils::findOrDefault(result, [&requiredAbi, &alternateAbi](const ToolChain *tc) {
            return requiredAbi == tc->targetAbi()
                    || (requiredAbi.wordWidth() == 64 && tc->targetAbi() == alternateAbi);
        });
        if (!abiTc) {
            qDeleteAll(result);
            result.clear();
        }
    }

    return result;
}

QList<ToolChain *> ArduinoToolChainFactory::autoDetectToolChain(const FileName &compilerPath,
                                                                const Core::Id language,
                                                                const Abi &requiredAbi)
{
    QList<ToolChain *> result;

    Environment systemEnvironment = Environment::systemEnvironment();
    GccToolChain::addCommandPathToEnvironment(compilerPath, systemEnvironment);
    const FileName localCompilerPath = findLocalCompiler(compilerPath, systemEnvironment);
    Macros macros
            = gccPredefinedMacros(localCompilerPath, gccPredefinedMacrosOptions(language),
                                  systemEnvironment.toStringList());
    const GccToolChain::DetectedAbisResult detectedAbis = guessGccAbi(localCompilerPath,
                                                                      systemEnvironment.toStringList(),
                                                                      macros);

    const QList<Abi> abiList = detectedAbis.supportedAbis;
    if (!requiredAbi.isNull() && !abiList.contains(requiredAbi)) {
        if (requiredAbi.wordWidth() != 64
                || !abiList.contains(Abi(requiredAbi.architecture(), requiredAbi.os(), requiredAbi.osFlavor(),
                                         requiredAbi.binaryFormat(), 32)))
            return result;
    }

    for (const Abi &abi : abiList) {
        std::unique_ptr<ArduinoToolChain> tc(new ArduinoToolChain(ToolChain::AutoDetection));
        if (!tc)
            return result;

        tc->setLanguage(language);
        //tc->m_predefinedMacrosCache->insert(QStringList(), macros);
        tc->setCompilerCommand(compilerPath);
        tc->setSupportedAbis(detectedAbis.supportedAbis);
        tc->setTargetAbi(abi);
        tc->setOriginalTargetTriple(detectedAbis.originalTargetTriple);
        tc->setDisplayName(tc->defaultDisplayName()); // reset displayname
        //qDebug() << Q_FUNC_INFO << tc->systemHeaderPaths()

        result.append(tc.release());
    }
    return result;
}

bool ArduinoToolChainFactory::canCreate()
{
    return true;
}

ToolChain *ArduinoToolChainFactory::create(Core::Id l)
{
    return new ArduinoToolChain(l, ToolChain::ManualDetection);
}


// --------------------------------------------------------------------------
// ToolChainConfigWidget
// --------------------------------------------------------------------------

ArduinoToolChainConfigWidget::ArduinoToolChainConfigWidget(ArduinoToolChain *tc)
    : ToolChainConfigWidget(tc)
    , m_compilerCommand(new PathChooser)
{
    m_compilerCommand->setExpectedKind(PathChooser::File);
    m_compilerCommand->setFileName(tc->compilerCommand());
    m_mainLayout->addRow(tr("Compiler path:"), m_compilerCommand);

    connect(m_compilerCommand, &PathChooser::rawPathChanged,
            this, &ArduinoToolChainConfigWidget::handleCompilerCommandChange);
}

ArduinoToolChainConfigWidget::~ArduinoToolChainConfigWidget()
{

}

void ArduinoToolChainConfigWidget::handleCompilerCommandChange()
{
    // TODO : validate the compiler

    emit dirty();
}

} // namespace Internal
} // namespace Arduino

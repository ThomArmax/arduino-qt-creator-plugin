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
#include "arduinosettings.h"
#include "arduinoconstants.h"
#include "toolchain/arduinotoolchain.h"

#include <QFileInfo>

#include <utils/fileutils.h>

#include <coreplugin/icore.h>

#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/toolchainmanager.h>
#include <projectexplorer/toolchain.h>
#include <projectexplorer/kit.h>
#include <projectexplorer/kitinformation.h>
#include <projectexplorer/kitmanager.h>

#include <qtsupport/baseqtversion.h>
#include <qtsupport/qtversionmanager.h>
#include <qtsupport/qtkitinformation.h>

#include <debugger/debuggeritem.h>
#include <debugger/debuggeritemmanager.h>
#include <debugger/debuggerkitinformation.h>

using namespace Utils;
using namespace ProjectExplorer;
using namespace QtSupport;
using namespace Debugger;

namespace Arduino {
namespace Internal {

namespace {
    // Settings
    const QLatin1String SettingsGroup("ArduinoConfiguration");
    const QLatin1String SdkLocationKey("Arduino.SdkLocation");
    const QLatin1String CreateKitKey("Arduino.CreateKit");

    // Paths
    const QLatin1String BoardsFileSuffix("/hardware/arduino/avr/boards.txt");
    const QLatin1String ArduinoSdkHeadersPathSuffix("/hardware/arduino/avr/cores/arduino/");
    const QLatin1String AvrBinPathSuffix("/hardware/tools/avr/bin/");
    const QLatin1String AvrIncludePathSuffix("/hardware/tools/avr/avr/include/");
}

/// Settings instance
ArduinoSettings *ArduinoSettings::m_instance = nullptr;

/**
 * @brief Default constructor
 */
ArduinoSettings::ArduinoSettings()
    : m_sdkLocation()
    , m_createKit(false)
{
    m_instance = this;
}

/**
 * @brief Loads the settings
 */
void ArduinoSettings::load()
{
    QSettings *settings = Core::ICore::settings();
    settings->beginGroup(SettingsGroup);

    setSdkLocation(FileName::fromString(settings->value(SdkLocationKey).toString()));
    setAutoCreateKit(settings->value(CreateKitKey).toBool());

    settings->endGroup();
    QString dummy;
    if (m_createKit && validate(m_sdkLocation.toString(), dummy))
        createTools();
}

/**
 * @brief Saves the settings
 */
void ArduinoSettings::save() const
{
    QSettings *settings = Core::ICore::settings();
    settings->beginGroup(SettingsGroup);

    settings->setValue(SdkLocationKey, m_sdkLocation.toString());
    settings->setValue(CreateKitKey, m_createKit);

    settings->endGroup();
}

/**
 * @brief Returns the root path of Arduino SDK
 * @return the root path of Arduino SDK
 */
Utils::FileName ArduinoSettings::sdkLocation() const
{
    return m_sdkLocation;
}

/**
 * @brief Sets the Arduino SDK root path
 * @param sdk
 */
void ArduinoSettings::setSdkLocation(const Utils::FileName &sdk)
{
    m_sdkLocation = sdk;
}

/**
 * @brief Tells whether or not the Arduino kit must be automatically created
 * @return `true` if automatically created; `false` otherwise
 */
bool ArduinoSettings::isAutoCreateKitEnabled() const
{
    return m_createKit;
}

/**
 * @brief Enables/Disables the automatic Arduino kit creation
 * @param enabled
 */
void ArduinoSettings::setAutoCreateKit(bool enabled)
{
    m_createKit = enabled;
}

/**
 * @brief Returns the absolute path the the Arduino `boards.txt` file
 * @return the absolute path the the Arduino `boards.txt` file
 */
Utils::FileName ArduinoSettings::boardsFile() const
{
    QFileInfo fileInfo(m_sdkLocation.toString() + BoardsFileSuffix);
    return Utils::FileName(fileInfo);
}

/**
 * @brief Returns the absolute path of the Arduino SDK headers
 * @return the absolute path of the Arduino SDK headers
 */
Utils::FileName ArduinoSettings::sdkIncludePath() const
{
    QFileInfo fileInfo(m_sdkLocation.toString() + ArduinoSdkHeadersPathSuffix);
    return Utils::FileName(fileInfo);
}

/**
 * @brief Returns the absolute path of AVR bins
 * @return the absolute path of AVR bins
 */
Utils::FileName ArduinoSettings::avrBinPath() const
{
    QFileInfo fileInfo(m_sdkLocation.toString() + AvrBinPathSuffix);
    return Utils::FileName(fileInfo);
}

/**
 * @brief Returns the absolute path of AVR headers
 * @return the absolute path of AVR headers
 */
Utils::FileName ArduinoSettings::avrIncludePath() const
{
    QFileInfo fileInfo(m_sdkLocation.toString() + AvrIncludePathSuffix);
    return Utils::FileName(fileInfo);
}

/**
 * @brief Validate the given Arduino SDK path
 * @param[in] path      Arduino SDK path to be validated
 * @param[out] errorStr Set by the method if validation failed
 * @return `true` is successfully validated; `false` otherwise
 */
bool ArduinoSettings::validate(const QString &path, QString &errorStr)
{
    QFileInfo infos(path);
    if (!infos.isDir())
    {
        errorStr = QObject::tr("Given path is not a directory");
        return false;
    }

    infos.setFile(path + BoardsFileSuffix);
    if (!infos.exists())
    {
        errorStr = QObject::tr("Could not find") + " " + infos.absoluteFilePath();
        return false;
    }

    infos.setFile(path + ArduinoSdkHeadersPathSuffix + "/Arduino.h");
    if (!infos.exists())
    {
        errorStr = QObject::tr("Could not find") + " " + infos.absoluteFilePath();
        return false;
    }

    infos.setFile(path + AvrBinPathSuffix);
    if (!infos.exists())
    {
        errorStr = QObject::tr("Could not find") + " " + infos.absoluteFilePath();
        return false;
    }

    infos.setFile(path + AvrIncludePathSuffix);
    if (!infos.exists())
    {
        errorStr = QObject::tr("Could not find") + " " + infos.absoluteFilePath();
        return false;
    }

    return true;
}

/**
 * @brief Creates the tools (toolchain, kit) for Arduino/AVR
 */
void ArduinoSettings::createTools()
{
    ArduinoToolChain * tc = createToolchain();
    createKit(tc);
}

/**
 * @brief Creates the Arduino toolchain and returns it
 * @note This method registers the created toolchain
 * @return create toolchain
 */
ArduinoToolChain * ArduinoSettings::createToolchain()
{
    ArduinoToolChain * toolChain = new ArduinoToolChain(ToolChain::AutoDetection);
    toolChain->setLanguage(ProjectExplorer::Constants::CXX_LANGUAGE_ID);
    toolChain->setDisplayName(QCoreApplication::translate("Arduino::Internal::ArduinoSettings", "AVR GCC (C++ compiler)"));
    ToolChainManager::registerToolChain(toolChain);

    return toolChain;
}

/**
 * @brief Creates the Arduino kit with the given toolchain and returns it
 * @note This method registers the created kit
 * @param toolChain
 * @return create kit
 */
Kit * ArduinoSettings::createKit(ArduinoToolChain *toolChain)
{
    Kit *kit = new Kit;

    ToolChainKitInformation::setToolChain(kit, toolChain);
    ToolChainKitInformation::clearToolChain(kit, ProjectExplorer::Constants::C_LANGUAGE_ID);

    // No Qt configuration
    QtKitInformation::setQtVersion(kit, nullptr);

    // Set device type
    DeviceTypeKitInformation::setDeviceTypeId(kit, Constants::ARDUINO_OS_TYPE);

    kit->setUnexpandedDisplayName(QCoreApplication::translate("Arduino::Internal::ArduinoSettings", "Kit for Arduino"));

    kit->setAutoDetected(true);
    kit->setAutoDetectionSource(m_sdkLocation.toString());
    kit->setMutable(DeviceKitInformation::id(), true);

    kit->setSticky(ToolChainKitInformation::id(), true);
    kit->setSticky(DeviceTypeKitInformation::id(), true);
    kit->setSticky(SysRootKitInformation::id(), true);
    kit->setSticky(DebuggerKitInformation::id(), true);
    kit->setSticky(QtKitInformation::id(), true);

//    kit->setSticky(QmakeProjectManager::QmakeKitInformation::id(), true);

    // add kit with device not sticky
    KitManager::registerKit(kit);
    return kit;
}

/**
 * @brief Validates the current Arduino SDK path
 * @param[out] errorStr Set by the method if validation failed
 * @return `true` is successfully validated; `false` otherwise
 */
bool ArduinoSettings::validate(QString &errorStr) const
{
    return validate(m_sdkLocation.toString(), errorStr);
}

/**
 * @brief Returns the instance of the settings
 * @return the instance of the settings
 */
ArduinoSettings *ArduinoSettings::instance()
{
    if (!m_instance)
        m_instance = new ArduinoSettings();
    return m_instance;
}

} // namespace Internal
} // namespace Arduino

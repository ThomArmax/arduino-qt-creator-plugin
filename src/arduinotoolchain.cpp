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
#include "arduinoconstants.h"

#include "projectexplorer/toolchain.h"
#include "projectexplorer/projectexplorerconstants.h"
#include "utils/pathchooser.h"

#include <QLabel>
#include <QFormLayout>

using namespace ProjectExplorer;
using namespace Utils;

namespace Arduino {
namespace Internal {

static QStringList reinterpretOptions(const QStringList &args)
{
    return args;
}

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
    return ArduinoToolChainFactory::tr("Arduino");
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
    setDisplayName(tr("Arduino"));
}

QSet<Core::Id> ArduinoToolChainFactory::supportedLanguages() const
{
    return {ProjectExplorer::Constants::C_LANGUAGE_ID};
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
    m_compilerCommand->setFileName(FileName::fromString("xtensa-lx106-elf-gcc"));
    m_mainLayout->addRow(tr("Compiler path:"), m_compilerCommand);

    connect(m_compilerCommand, &PathChooser::rawPathChanged,
            this, &ArduinoToolChainConfigWidget::handleCompilerCommandChange);
}

void ArduinoToolChainConfigWidget::handleCompilerCommandChange()
{
    // TODO : validate the compiler

    emit dirty();
}

} // namespace Internal
} // namespace Arduino

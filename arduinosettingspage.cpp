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
#include "arduinosettingspage.h"
#include "arduinoconstants.h"
#include "arduinosettingswidget.h"

#include <coreplugin/icore.h>
#include <projectexplorer/projectexplorerconstants.h>

#include <QCoreApplication>
#include <QMessageBox>

namespace Arduino {
namespace Internal {

ArduinoSettingsPage::ArduinoSettingsPage(QObject* parent) :
    Core::IOptionsPage(parent)
{
    setId(Core::Id(Constants::ARDUINO_SETTINGS_ID));
    setDisplayName(tr("Arduino"));
    setCategory(ProjectExplorer::Constants::DEVICE_SETTINGS_CATEGORY);
    setDisplayCategory(QCoreApplication::translate("ProjectExplorer", ProjectExplorer::Constants::DEVICE_SETTINGS_TR_CATEGORY));
}

QWidget* ArduinoSettingsPage::widget()
{
    if (!m_widget)
        m_widget = new ArduinoSettingsWidget;

    return m_widget;
}

void ArduinoSettingsPage::apply()
{
    if (m_widget)
        m_widget->saveSettings();
}

void ArduinoSettingsPage::finish()
{
    delete m_widget;
}

} // namespace Internal
} // namespace Arduino

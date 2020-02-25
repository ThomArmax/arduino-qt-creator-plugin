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
#include "arduinosettingswidget.h"
#include "arduinosettings.h"
#include "ui_arduinosettingswidget.h"

namespace Arduino {
namespace Internal {

ArduinoSettingsWidget::ArduinoSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ArduinoSettingsWidget)
{
    ui->setupUi(this);

    ArduinoSettings::instance()->load();

    ui->sdkLocationPathChooser->setFileName(ArduinoSettings::instance()->sdkLocation());
    ui->checkBoxAutoCreateKit->setChecked(ArduinoSettings::instance()->isAutoCreateKitEnabled());
}

ArduinoSettingsWidget::~ArduinoSettingsWidget()
{
    delete ui;
}

void ArduinoSettingsWidget::saveSettings()
{
    ArduinoSettings::instance()->setAutoCreateKit(ui->checkBoxAutoCreateKit->isChecked());
    ArduinoSettings::instance()->setSdkLocation(ui->sdkLocationPathChooser->fileName());
    ArduinoSettings::instance()->save();
}

} // namespace Internal
} // namespace ArduinoToolchain

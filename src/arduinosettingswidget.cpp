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

#include <utils/fancylineedit.h>

#include <QDebug>
#include <QMessageBox>

namespace Arduino {
namespace Internal {

ArduinoSettingsWidget::ArduinoSettingsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ArduinoSettingsWidget)
    , m_settings(ArduinoSettings::instance())
{
    ui->setupUi(this);
    ui->sdkLocationPathChooser->setValidationFunction(&ArduinoSettingsWidget::validateSdkPath);

    m_settings->load();
    updateUi();

    connect(ui->sdkLocationPathChooser, &Utils::PathChooser::pathChanged, this, &ArduinoSettingsWidget::onSdkPathChanged);
}

ArduinoSettingsWidget::~ArduinoSettingsWidget()
{
    delete ui;
}

void ArduinoSettingsWidget::saveSettings()
{
    ArduinoSettings::instance()->setAutoCreateKit(ui->checkBoxAutoCreateKit->isChecked());
    ArduinoSettings::instance()->setSdkLocation(ui->sdkLocationPathChooser->fileName());

    validateSettings(true);

    m_settings->save();
    updateUi();
}

bool ArduinoSettingsWidget::validateSettings(bool showPopup)
{
    QString errorStr;
    bool valid = m_settings->validate(errorStr);
    if (!valid && showPopup)
    {
        QMessageBox::warning(this, "Error", QString("Given path could not be validated.\r\n"
                                            "Reason : %1").arg(errorStr));
    }

    return valid;
}

bool ArduinoSettingsWidget::validateSdkPath(Utils::FancyLineEdit *edit, QString *errorMessage)
{
    return ArduinoSettings::instance()->validate(edit->text(), *errorMessage);
}

// -- Slots

void ArduinoSettingsWidget::updateUi()
{
    ui->sdkLocationPathChooser->setFileName(m_settings->sdkLocation());
    ui->checkBoxAutoCreateKit->setChecked(m_settings->isAutoCreateKitEnabled());

    QString errorStr;
    if (m_settings->validate(errorStr))
    {
        ui->labelArduinoBoardsFile->setText(m_settings->boardsFile().toString());
        ui->labelArduinoHeaders->setText(m_settings->sdkIncludePath().toString());
        ui->labelAvrHeaders->setText(m_settings->avrIncludePath().toString());
        ui->labelAvrBins->setText(m_settings->avrBinPath().toString());
    }
    else
    {
        ui->labelArduinoBoardsFile->setText("N/A");
        ui->labelArduinoHeaders->setText("N/A");
        ui->labelAvrHeaders->setText("N/A");
        ui->labelAvrBins->setText("N/A");
    }
}

void ArduinoSettingsWidget::onSdkPathChanged(const QString &path)
{
    qDebug() << Q_FUNC_INFO << path;
}

} // namespace Internal
} // namespace ArduinoToolchain

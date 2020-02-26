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
#ifndef ARDUINOSETTINGSWIDGET_H
#define ARDUINOSETTINGSWIDGET_H

#include <QWidget>

namespace Ui {
class ArduinoSettingsWidget;
}

namespace Utils {
class FancyLineEdit;
}

namespace Arduino {
namespace Internal {

class ArduinoSettings;

class ArduinoSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ArduinoSettingsWidget(QWidget *parent = 0);
    ~ArduinoSettingsWidget();

    void saveSettings();

private:
    bool validateSettings(bool showPopup);
    static bool validateSdkPath(Utils::FancyLineEdit *edit, QString *errorMessage);

private slots:
    void updateUi();
    void onSdkPathChanged(const QString &path);

private:
    Ui::ArduinoSettingsWidget *ui;
    ArduinoSettings * m_settings;
};

}
}

#endif // ARDUINOSETTINGSWIDGET_H

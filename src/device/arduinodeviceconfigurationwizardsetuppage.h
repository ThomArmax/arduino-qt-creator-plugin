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
#ifndef ARDUINODEVICECONFIGURATIONWIZARDSETUPPAGE_H
#define ARDUINODEVICECONFIGURATIONWIZARDSETUPPAGE_H

#include <QWizardPage>

QT_BEGIN_NAMESPACE
class QLineEdit;
QT_END_NAMESPACE

namespace Arduino {
namespace Internal {

class ArduinoDeviceConfigurationWizardSetupPage : public QWizardPage
{
public:
    ArduinoDeviceConfigurationWizardSetupPage(QWidget *parent = nullptr);

    void initializePage();
    bool isComplete() const;
    QString configurationName() const;

    virtual QString defaultConfigurationName() const;

private:
    QLineEdit *m_nameLineEdit;
};

} // namespace Internal
} // namespace Arduino

#endif // ARDUINODEVICECONFIGURATIONWIZARDSETUPPAGE_H

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
#include "arduinodeviceconfigurationwizardsetuppage.h"

#include <QFormLayout>
#include <QLineEdit>

namespace Arduino {
namespace Internal {

ArduinoDeviceConfigurationWizardSetupPage::ArduinoDeviceConfigurationWizardSetupPage(
        QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Set up Arduino device"));

    auto formLayout = new QFormLayout(this);
    formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    m_nameLineEdit = new QLineEdit(this);
    formLayout->addRow(tr("Name:"), m_nameLineEdit);

    connect(m_nameLineEdit, &QLineEdit::textChanged,
            this, &ArduinoDeviceConfigurationWizardSetupPage::completeChanged);
}

void ArduinoDeviceConfigurationWizardSetupPage::initializePage()
{
    m_nameLineEdit->setText(defaultConfigurationName());
}

bool ArduinoDeviceConfigurationWizardSetupPage::isComplete() const
{
    return !configurationName().isEmpty();
}

QString ArduinoDeviceConfigurationWizardSetupPage::configurationName() const
{
    return m_nameLineEdit->text().trimmed();
}

QString ArduinoDeviceConfigurationWizardSetupPage::defaultConfigurationName() const
{
    return tr("Arduino Device");
}

} // namespace Internal
} // namespace Arduino

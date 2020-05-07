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
#include "arduinopotentialkit.h"
#include "../arduinoconstants.h"

#include <app/app_version.h>

#include <coreplugin/icore.h>
#include <coreplugin/coreicons.h>
#include <coreplugin/icore.h>

#include <projectexplorer/kitmanager.h>
#include <projectexplorer/kit.h>
#include <projectexplorer/kitinformation.h>

#include <utils/utilsicons.h>

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

namespace Arduino {
namespace Internal {

QString ArduinoPotentialKit::displayName() const
{
    return tr("Configure Arduino...");
}

void ArduinoPotentialKit::executeFromMenu()
{
    Core::ICore::showOptionsDialog(Constants::AVR_DEVICE_ID);
}

QWidget *ArduinoPotentialKit::createWidget(QWidget *parent) const
{
    if (!isEnabled())
        return nullptr;
    return new ArduinoPotentialKitWidget(parent);
}

bool ArduinoPotentialKit::isEnabled() const
{
    QList<ProjectExplorer::Kit *> kits = ProjectExplorer::KitManager::kits();
    foreach (ProjectExplorer::Kit *kit, kits) {
        Core::Id deviceId = ProjectExplorer::DeviceKitInformation::deviceId(kit);
        qDebug() << Q_FUNC_INFO << "kit device id :" << deviceId;
        if (kit->isAutoDetected()
                && deviceId == Core::Id(Constants::AVR_DEVICE_ID)
                && !kit->isSdkProvided()) {
            return false;
        }
    }

    return true;
//    return QtSupport::QtVersionManager::version([](const QtSupport::BaseQtVersion *v) {
//        return v->isValid() && v->type() == QString::fromLatin1(Constants::ANDROIDQT);
//    });
}

ArduinoPotentialKitWidget::ArduinoPotentialKitWidget(QWidget *parent)
    : Utils::DetailsWidget(parent)
{
    setSummaryText(QLatin1String("<b>Arduino has not been configured. Create Arduino kits.</b>"));
    setIcon(Utils::Icons::WARNING.icon());
    //detailsWidget->setState(Utils::DetailsWidget::NoSummary);
    auto mainWidget = new QWidget(this);
    setWidget(mainWidget);

    auto layout = new QGridLayout(mainWidget);
    layout->setMargin(0);
    auto label = new QLabel;
    label->setText(tr("%1 needs additional settings to enable Arduino support."
                      " You can configure those settings in the Options dialog.")
                   .arg(Core::Constants::IDE_DISPLAY_NAME));
    label->setWordWrap(true);
    layout->addWidget(label, 0, 0, 1, 2);

    auto openOptions = new QPushButton;
    openOptions->setText(Core::ICore::msgShowOptionsDialog());
    openOptions->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    layout->addWidget(openOptions, 1, 1);

    connect(openOptions, &QAbstractButton::clicked,
            this, &ArduinoPotentialKitWidget::openOptions);

//    connect(ArduinoSettings::instance(), &ArduinoSettings::updated,
//            this, &ArduinoPotentialKitWidget::recheck);
}

void ArduinoPotentialKitWidget::openOptions()
{
    Core::ICore::showOptionsDialog(Constants::ARDUINO_SETTINGS_ID, this);
}

void ArduinoPotentialKitWidget::recheck()
{
    QList<ProjectExplorer::Kit *> kits = ProjectExplorer::KitManager::kits();
    foreach (ProjectExplorer::Kit *kit, kits) {
        Core::Id deviceId = ProjectExplorer::DeviceKitInformation::deviceId(kit);
        if (kit->isAutoDetected()
                && deviceId == Core::Id(Constants::AVR_DEVICE_ID)
                && !kit->isSdkProvided()) {
            setVisible(false);
            return;
        }
    }
}

} // namespace Internal
} // namespace Arduino

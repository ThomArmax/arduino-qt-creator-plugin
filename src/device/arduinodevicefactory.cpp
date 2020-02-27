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
#include "arduinodevicefactory.h"
#include "../arduinoconstants.h"
#include "arduinodevice.h"
#include "arduinodeviceconfigurationwizard.h"

#include <QIcon>
#include <utils/icon.h>
#include <utils/qtcassert.h>

namespace Arduino {
namespace Internal {

ArduinoDeviceFactory::ArduinoDeviceFactory(QObject *parent) :
    ProjectExplorer::IDeviceFactory(parent)
{
}

QString ArduinoDeviceFactory::displayNameForId(Core::Id type) const
{
    Q_UNUSED(type);
    return tr("Arduino Device");
}

QList<Core::Id> ArduinoDeviceFactory::availableCreationIds() const
{
    return { Constants::ARDUINO_OS_TYPE };
}

QIcon ArduinoDeviceFactory::iconForId(Core::Id type) const
{
    Q_UNUSED(type)
    using namespace Utils;
    static const QIcon icon = Icon::combinedIcon({Icon({{":/wizards/arduino_logo.png", Theme::PanelTextColorDark}}, Icon::Tint),
                                                  Icon({{":/wizards/arduino_logo.png", Theme::IconsBaseColor}})});
    return icon;
}

bool ArduinoDeviceFactory::canCreate() const
{
    return false;
}

ProjectExplorer::IDevice::Ptr ArduinoDeviceFactory::create(Core::Id id) const
{
    Q_UNUSED(id);
    ArduinoDeviceConfigurationWizard wizard;
    if (wizard.exec() != QDialog::Accepted)
        return ProjectExplorer::IDevice::Ptr();
    return wizard.device();
}

bool ArduinoDeviceFactory::canRestore(const QVariantMap &map) const
{
    return ProjectExplorer::IDevice::typeFromMap(map) == Constants::ARDUINO_OS_TYPE;
}

ProjectExplorer::IDevice::Ptr ArduinoDeviceFactory::restore(const QVariantMap &map) const
{
    QTC_ASSERT(canRestore(map), return ArduinoDevice::Ptr());
    const ArduinoDevice::Ptr device = ArduinoDevice::create();
    device->fromMap(map);
    return device;
}

Core::Id ArduinoDeviceFactory::deviceType()
{
    return Core::Id(Constants::ARDUINO_OS_TYPE);
}

} // namespace Internal
} // namespace ArduinoToolchain

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
#include "arduinodeviceconfigurationfactory.h"
#include "arduinodeviceconfigurationwizard.h"
#include "../arduinoconstants.h"
#include "arduinodevice.h"

#include <utils/icon.h>
#include <utils/qtcassert.h>

#include <QIcon>

using namespace ProjectExplorer;

namespace Arduino {
namespace Internal {

ArduinoDeviceConfigurationFactory::ArduinoDeviceConfigurationFactory(QObject *parent)
    : IDeviceFactory(parent)
{
}

QString ArduinoDeviceConfigurationFactory::displayNameForId(Core::Id type) const
{
    return type == Constants::ARDUINO_OS_TYPE ? tr("Arduino Device") : QString();
}

QList<Core::Id> ArduinoDeviceConfigurationFactory::availableCreationIds() const
{
    return QList<Core::Id>() << Core::Id(Constants::ARDUINO_OS_TYPE);
}

QIcon ArduinoDeviceConfigurationFactory::iconForId(Core::Id type) const
{
    Q_UNUSED(type)
    using namespace Utils;
    static const QIcon icon =
            Icon::combinedIcon({Icon({{":/wizards/arduino_logo.png",
                                       Theme::PanelTextColorDark}}, Icon::Tint),
                                Icon({{":/wizards/arduino_logo.png",
                                       Theme::IconsBaseColor}})});
    return icon;
}

IDevice::Ptr ArduinoDeviceConfigurationFactory::create(Core::Id id) const
{
    QTC_ASSERT(id == Constants::ARDUINO_OS_TYPE, return IDevice::Ptr());
    ArduinoDeviceConfigurationWizard wizard;
    if (wizard.exec() != QDialog::Accepted)
        return IDevice::Ptr();
    return wizard.device();
}

bool ArduinoDeviceConfigurationFactory::canRestore(const QVariantMap &map) const
{
   return IDevice::typeFromMap(map) == Constants::ARDUINO_OS_TYPE;
}

IDevice::Ptr ArduinoDeviceConfigurationFactory::restore(const QVariantMap &map) const
{
    QTC_ASSERT(canRestore(map), return IDevice::Ptr());
    const IDevice::Ptr device = ArduinoDevice::create();
    device->fromMap(map);
    return device;
}

} // namespace Internal
} // namespace Arduino

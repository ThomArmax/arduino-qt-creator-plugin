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
#include "arduinodevice.h"
#include "arduinodeviceconfigurationwidget.h"

#include <coreplugin/id.h>
#include <utils/qtcassert.h>

#include <QCoreApplication>

using namespace ProjectExplorer;

namespace Arduino {
namespace Internal {

ArduinoDevice::Ptr ArduinoDevice::create()
{
    return Ptr(new ArduinoDevice);
}

ArduinoDevice::Ptr ArduinoDevice::create(const QString &name, Core::Id type, MachineType machineType, Origin origin, Core::Id id)
{
    return Ptr(new ArduinoDevice(name, type, machineType, origin, id));
}

ArduinoDevice::Ptr ArduinoDevice::create(const ArduinoDevice &other)
{
    return Ptr(new ArduinoDevice(other));
}

ArduinoDevice::~ArduinoDevice()
{

}

void ArduinoDevice::fromMap(const QVariantMap &map)
{
    IDevice::fromMap(map);
}

QVariantMap ArduinoDevice::toMap() const
{
    QVariantMap map = IDevice::toMap();
    return map;
}

ArduinoDevice::IDevice::Ptr ArduinoDevice::clone() const
{
    return Ptr(new ArduinoDevice(*this));
}

DeviceProcessSignalOperation::Ptr ArduinoDevice::signalOperation() const
{
    return DeviceProcessSignalOperation::Ptr();
}

QString ArduinoDevice::displayType() const
{
    return QCoreApplication::translate("Arduino::Internal::ArduinoDevice", "Arduino");
}

IDeviceWidget *ArduinoDevice::createWidget()
{
    return new ArduinoDeviceConfigurationWidget(sharedFromThis());
}

QList<Core::Id> ArduinoDevice::actionIds() const
{
    return QList<Core::Id>(); // no actions
}

QString ArduinoDevice::displayNameForActionId(Core::Id actionId) const
{
    QTC_ASSERT(actionIds().contains(actionId), return QString());
    return QString();
}

void ArduinoDevice::executeAction(Core::Id actionId, QWidget *parent)
{
    QTC_ASSERT(actionIds().contains(actionId), return);
    Q_UNUSED(parent);
}

Utils::OsType ArduinoDevice::osType() const
{
    return Utils::OsTypeOther;
}

DeviceProcess *ArduinoDevice::createProcess(QObject *parent) const
{
    Q_UNUSED(parent);
    return nullptr;
}

ArduinoDevice::ArduinoDevice(const QString &name, Core::Id type, MachineType machineType, Origin origin, Core::Id id)
    : IDevice(type, origin, machineType, id)
{
    setDisplayName(name);
}

ArduinoDevice::ArduinoDevice(const ArduinoDevice &other)
    : IDevice(other)
{

}

} // namespace Internal
} // namespace Arduino

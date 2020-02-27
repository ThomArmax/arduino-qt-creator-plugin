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
#ifndef ARDUINODEVICE_H
#define ARDUINODEVICE_H

#include <projectexplorer/devicesupport/idevice.h>

namespace Arduino {
namespace Internal {

class ArduinoDevice : public ProjectExplorer::IDevice
{
public:
    typedef QSharedPointer<ArduinoDevice> Ptr;
    typedef QSharedPointer<const ArduinoDevice> ConstPtr;

    static Ptr create();
    static Ptr create(const QString &name, Core::Id type, MachineType machineType,
                      Origin origin = ManuallyAdded, Core::Id id = Core::Id());
    static Ptr create(const ArduinoDevice &other);

    ~ArduinoDevice();
    QString displayType() const override;
    ProjectExplorer::IDeviceWidget *createWidget() override;
    QList<Core::Id> actionIds() const override;
    QString displayNameForActionId(Core::Id actionId) const override;
    void executeAction(Core::Id actionId, QWidget *parent) override;
    Utils::OsType osType() const override;
    ProjectExplorer::IDevice::Ptr clone() const override;

    ProjectExplorer::DeviceProcessSignalOperation::Ptr signalOperation() const override;

    bool canCreateProcess() const override { return false; }
    ProjectExplorer::DeviceProcess *createProcess(QObject *parent) const override;

    virtual void fromMap(const QVariantMap &map) override;
    virtual QVariantMap toMap() const override;

protected:
    ArduinoDevice() {}
    ArduinoDevice(const QString &name, Core::Id type, MachineType machineType, Origin origin, Core::Id id);
    ArduinoDevice(const ArduinoDevice &other);

private:
    ArduinoDevice &operator=(const ArduinoDevice &);
};

} // namespace Internal
} // namespace Arduino

#endif // ARDUINODEVICE_H

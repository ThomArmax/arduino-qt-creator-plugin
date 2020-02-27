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
#ifndef ARDUINODEVICECONFIGURATIONFACTORY_H
#define ARDUINODEVICECONFIGURATIONFACTORY_H

#include <projectexplorer/devicesupport/idevicefactory.h>

namespace Arduino {
namespace Internal {

class ArduinoDeviceConfigurationFactory : public ProjectExplorer::IDeviceFactory
{
    Q_OBJECT

public:
    ArduinoDeviceConfigurationFactory(QObject *parent = 0);

    QString displayNameForId(Core::Id type) const;
    QList<Core::Id> availableCreationIds() const;
    QIcon iconForId(Core::Id type) const;

    ProjectExplorer::IDevice::Ptr create(Core::Id id) const;
    bool canRestore(const QVariantMap &map) const;
    ProjectExplorer::IDevice::Ptr restore(const QVariantMap &map) const;
};

} // namespace Internal
} // namespace Arduino


#endif // ARDUINODEVICECONFIGURATIONFACTORY_H

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
#include "arduinosettings.h"

#include <utils/fileutils.h>
#include <coreplugin/icore.h>

using namespace Utils;

namespace Arduino {
namespace Internal {

namespace {
    const QLatin1String SettingsGroup("ArduinoConfiguration");
    const QLatin1String SdkLocationKey("SdkLocation");
    const QLatin1String CreateKitKey("CreateKit");
}

ArduinoSettings *ArduinoSettings::m_instance = nullptr;

ArduinoSettings::ArduinoSettings()
    : m_sdkLocation()
    , m_createKit(false)
{
    m_instance = this;
}

void ArduinoSettings::load()
{
    QSettings *settings = Core::ICore::settings();
    settings->beginGroup(SettingsGroup);

    setSdkLocation(FileName::fromString(settings->value(SdkLocationKey).toString()));
    setAutoCreateKit(settings->value(CreateKitKey).toBool());

    settings->endGroup();
}

void ArduinoSettings::save() const
{
    QSettings *settings = Core::ICore::settings();
    settings->beginGroup(SettingsGroup);

    settings->setValue(SdkLocationKey, m_instance->m_sdkLocation.toString());
    settings->setValue(CreateKitKey, m_instance->m_createKit);

    settings->endGroup();
}

Utils::FileName ArduinoSettings::sdkLocation() const
{
    return m_sdkLocation;
}

void ArduinoSettings::setSdkLocation(const Utils::FileName &sdk)
{
    m_sdkLocation = sdk;
}

bool ArduinoSettings::isAutoCreateKitEnabled() const
{
    return m_createKit;
}

void ArduinoSettings::setAutoCreateKit(bool enabled)
{
    m_createKit = enabled;
}

ArduinoSettings *ArduinoSettings::instance()
{
    if (!m_instance)
        m_instance = new ArduinoSettings();
    return m_instance;
}

} // namespace Internal
} // namespace Arduino

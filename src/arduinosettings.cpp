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

#include <QFileInfo>

#include <utils/fileutils.h>
#include <coreplugin/icore.h>

using namespace Utils;

namespace Arduino {
namespace Internal {

namespace {
    // Settings
    const QLatin1String SettingsGroup("ArduinoConfiguration");
    const QLatin1String SdkLocationKey("Arduino.SdkLocation");
    const QLatin1String CreateKitKey("Arduino.CreateKit");

    // Paths
    const QLatin1String BoardsFileSuffix("/hardware/arduino/avr/boards.txt");
    const QLatin1String ArduinoSdkHeadersPathSuffix("/hardware/arduino/avr/cores/arduino/");
    const QLatin1String AvrBinPathSuffix("/hardware/tools/avr/bin/");
    const QLatin1String AvrIncludePathSuffix("/hardware/tools/avr/avr/include/");
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

    settings->setValue(SdkLocationKey, m_sdkLocation.toString());
    settings->setValue(CreateKitKey, m_createKit);

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

Utils::FileName ArduinoSettings::boardsFile() const
{
    QFileInfo fileInfo(m_sdkLocation.toString() + BoardsFileSuffix);
    return Utils::FileName(fileInfo);
}

Utils::FileName ArduinoSettings::sdkIncludePath() const
{
    QFileInfo fileInfo(m_sdkLocation.toString() + ArduinoSdkHeadersPathSuffix);
    return Utils::FileName(fileInfo);
}

Utils::FileName ArduinoSettings::avrBinPath() const
{
    QFileInfo fileInfo(m_sdkLocation.toString() + AvrBinPathSuffix);
    return Utils::FileName(fileInfo);
}

Utils::FileName ArduinoSettings::avrIncludePath() const
{
    QFileInfo fileInfo(m_sdkLocation.toString() + AvrIncludePathSuffix);
    return Utils::FileName(fileInfo);
}

bool ArduinoSettings::validate(const QString &path, QString &errorStr)
{
    QFileInfo infos(path);
    if (!infos.isDir())
    {
        errorStr = QObject::tr("Given path is not a directory");
        return false;
    }

    infos.setFile(path + BoardsFileSuffix);
    if (!infos.exists())
    {
        errorStr = QObject::tr("Could not find") + " " + infos.absoluteFilePath();
        return false;
    }

    infos.setFile(path + ArduinoSdkHeadersPathSuffix + "/Arduino.h");
    if (!infos.exists())
    {
        errorStr = QObject::tr("Could not find") + " " + infos.absoluteFilePath();
        return false;
    }

    infos.setFile(path + AvrBinPathSuffix);
    if (!infos.exists())
    {
        errorStr = QObject::tr("Could not find") + " " + infos.absoluteFilePath();
        return false;
    }

    infos.setFile(path + AvrIncludePathSuffix);
    if (!infos.exists())
    {
        errorStr = QObject::tr("Could not find") + " " + infos.absoluteFilePath();
        return false;
    }

    return true;
}

bool ArduinoSettings::validate(QString &errorStr) const
{
    return validate(m_sdkLocation.toString(), errorStr);
}

ArduinoSettings *ArduinoSettings::instance()
{
    if (!m_instance)
        m_instance = new ArduinoSettings();
    return m_instance;
}

} // namespace Internal
} // namespace Arduino

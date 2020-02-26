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
#ifndef ARDUINOSETTINGS_H
#define ARDUINOSETTINGS_H

#include <QSettings>
#include <utils/fileutils.h>

namespace Arduino {
namespace Internal {

/**
 * @brief Arduino SDK settings
 */
class ArduinoSettings
{
public:
    static ArduinoSettings *instance();

    void load();
    void save() const;

    Utils::FileName sdkLocation() const;
    void setSdkLocation(const Utils::FileName &sdk);

    bool isAutoCreateKitEnabled() const;
    void setAutoCreateKit(bool enabled);

    Utils::FileName boardsFile() const;
    Utils::FileName sdkIncludePath() const;
    Utils::FileName avrBinPath() const;
    Utils::FileName avrIncludePath() const;

    static bool validate(const QString &path, QString &errorStr);
    bool validate(QString &errorStr) const;

private:
    ArduinoSettings();

private:
    Utils::FileName m_sdkLocation; ///< Arduino SDK location
    bool m_createKit; ///< Auto create kit flag

    static ArduinoSettings *m_instance; ///< Settings instance

};

} // namespace Internal
} // namespace Arduino

#endif // ARDUINOSETTINGS_H

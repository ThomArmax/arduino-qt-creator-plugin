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
#pragma once

namespace Arduino {
namespace Constants {

const char ARDUINO_TOOLCHAIN_ID[] = "Avr.GccToolChain";

const char AVR_DEVICE_ID[] = "Avr.Device";
const char AVR_KIT_ID[] = "Avr.Kit";

const char ARDUINO_OS_TYPE[] = "Arduino.OsType";

// Arduino settings constants
const char ARDUINO_SETTINGS_ID[] = "Arduino.Configuration";

// Arduino tools menu constants
const char ARDUINO_TOOLS_MENU_ARDUINO_ID[]              = "Arduino.Tools.Menu";
const char ARDUINO_TOOLS_MENU_DOWNLOAD_ACTION[]         = "Arduino.Tools.Menu.Download.Action";
const char ARDUINO_TOOLS_MENU_SERIAL_MONITOR_ACTION[]   = "Arduino.Tools.Menu.SerialMonitor.Action";

// Arduino projects constants
const char ARDUINO_PROJECT_WIZARD_CATEGORY[]  = "Arduino.Projects.ArduinoProject";
const char ARDIUNO_PROJECT_WIZARD_CATEGORY_DISPLAY[] = QT_TRANSLATE_NOOP("ProjectExplorer", "Arduino");

// Mime types
const char INO_SOURCE_MIMETYPE[] = "text/x-ino-src";

} // namespace Constants
} // namespace Arduino

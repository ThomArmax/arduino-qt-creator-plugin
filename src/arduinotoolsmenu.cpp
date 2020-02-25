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
#include "arduinotoolsmenu.h"
#include "arduinoconstants.h"

#include <QAction>
#include <QObject>
#include <QMenu>
#include <QDebug>

#include <coreplugin/icore.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/actionmanager.h>

namespace Arduino {
namespace Internal {

ArduinoToolsMenu::ArduinoToolsMenu()
    : QObject()
{
    // Program download
    QAction *downloadAction = new QAction(tr("Download Program"), this);
    Core::Command *downloadCmd = Core::ActionManager::registerAction(downloadAction, Constants::ARDUINO_TOOLS_MENU_DOWNLOAD_ACTION, Core::Context(Core::Constants::C_GLOBAL));
    //cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Alt+Meta+A")));
    connect(downloadAction, &QAction::triggered, this, &ArduinoToolsMenu::triggerActionDownloadProgram);

    // Serial monitor
    QAction *serialMonitorAction = new QAction(tr("Open Serial Monitor"), this);
    Core::Command *serialMonitorCmd = Core::ActionManager::registerAction(serialMonitorAction, Constants::ARDUINO_TOOLS_MENU_SERIAL_MONITOR_ACTION, Core::Context(Core::Constants::C_GLOBAL));
    connect(serialMonitorAction, &QAction::triggered, this, &ArduinoToolsMenu::triggerActionOpenSerialMonitor);

    Core::ActionContainer *menu = Core::ActionManager::createMenu(Constants::ARDUINO_TOOLS_MENU_ARDUINO_ID);
    menu->menu()->setTitle(tr("Arduino"));
    menu->addAction(downloadCmd);
    menu->addAction(serialMonitorCmd);
    Core::ActionManager::actionContainer(Core::Constants::M_TOOLS)->addMenu(menu);
}

void ArduinoToolsMenu::triggerActionDownloadProgram()
{
    qDebug() << Q_FUNC_INFO;
}

void ArduinoToolsMenu::triggerActionOpenSerialMonitor()
{
    qDebug() << Q_FUNC_INFO;
}

} // namespace Internal
} // namespace Arduino

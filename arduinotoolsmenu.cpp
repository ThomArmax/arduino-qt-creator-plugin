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

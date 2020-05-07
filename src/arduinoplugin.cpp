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
#include "arduinoplugin.h"
#include "arduinoconstants.h"
#include "arduinosettingspage.h"
#include "arduinosettings.h"
#include "arduinotoolsmenu.h"
#include "toolchain/arduinotoolchain.h"
#include "toolchain/arduinopotentialkit.h"
#include "device/arduinodevicefactory.h"
#include "device/arduinodeviceconfigurationfactory.h"
#include "wizards/projects/arduino/arduinoprojectwizard.h"
#include "wizards/projects/arduino/arduinoprojectjsonfactory.h"
#include "wizards/projects/avr/avrprojectwizard.h"
#include "project/arduinoproject.h"

#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>
#include <projectexplorer/customwizard/customwizard.h>
#include <projectexplorer/projectmanager.h>
#include <projectexplorer/kitmanager.h>
#include <projectexplorer/jsonwizard/jsonwizardfactory.h>

#include <QAction>
#include <QMessageBox>
#include <QMainWindow>
#include <QMenu>

namespace Arduino {
namespace Internal {


ArduinoPlugin::ArduinoPlugin()
{
    // Create your members
}

ArduinoPlugin::~ArduinoPlugin()
{
    // Unregister objects from the plugin manager's object pool
    // Delete members
}

bool ArduinoPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    // Register objects in the plugin manager's object pool
    // Load settings
    // Add actions to menus
    // Connect to other plugins' signals
    // In the initialize function, a plugin can be sure that the plugins it
    // depends on have initialized their members.

    Q_UNUSED(arguments);
    Q_UNUSED(errorString);

    // Instanciate the ArduinoSettings
    new ArduinoSettings(this);

    // Register objects
    addAutoReleasedObject(new ArduinoSettingsPage);
    addAutoReleasedObject(new ArduinoToolsMenu);
    addAutoReleasedObject(new ArduinoToolChainFactory);
    addAutoReleasedObject(new ArduinoPotentialKit);
    addAutoReleasedObject(new ArduinoDeviceFactory);
    addAutoReleasedObject(new ArduinoDeviceConfigurationFactory);

    // Register Arduino project wizard
    Core::IWizardFactory::registerFactoryCreator([] {
        return QList<Core::IWizardFactory *> {
            new ArduinoProjectWizard,
            new AvrProjectWizard,
        };
    });

    JsonWizardFactory::addWizardPath(Utils::FileName::fromString(":/wizards/projects/"));

    // Register Arduino project
    ProjectExplorer::ProjectManager::registerProjectType<ArduinoProject>(Constants::ARDUINO_INO_PROJECT_MIMETYPE);

    return true;
}

void ArduinoPlugin::extensionsInitialized()
{
    // Retrieve objects from the plugin manager's object pool
    // In the extensionsInitialized function, a plugin can be sure that all
    // plugins that depend on it are completely initialized.
}

ExtensionSystem::IPlugin::ShutdownFlag ArduinoPlugin::aboutToShutdown()
{
    // Save settings
    // Disconnect from signals that are not needed during shutdown
    // Hide UI (if you add UI that is not in the main window directly)
    return SynchronousShutdown;
}

} // namespace Internal
} // namespace ArduinoToolchain

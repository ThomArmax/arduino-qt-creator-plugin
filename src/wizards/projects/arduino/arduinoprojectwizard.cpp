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
#include "arduinoprojectwizard.h"
#include "arduinoprojectwizarddialog.h"
#include "../../../arduinoconstants.h"

#include <utils/fileutils.h>

namespace Arduino {
namespace Internal {

using namespace Core;

ArduinoProjectWizard::ArduinoProjectWizard()
{
    setSupportedProjectTypes({Constants::ARDUINO_PROJECT_WIZARD_CATEGORY});
    setIcon(QIcon(QLatin1String(":/wizards/arduino_logo.png")));
    setDisplayName(tr("Arduino Project"));
    setId("Arduino.Project.Makefile");
    setDescription(tr("Creates an Arduino project"));
    setCategory(QLatin1String(Constants::ARDUINO_PROJECT_WIZARD_CATEGORY));
    setDisplayCategory(QLatin1String(Constants::ARDIUNO_PROJECT_WIZARD_CATEGORY_DISPLAY));
    setFlags(Core::IWizardFactory::PlatformIndependent);
}

BaseFileWizard *ArduinoProjectWizard::create(QWidget *parent, const WizardDialogParameters &parameters) const
{
    ArduinoProjectWizardDialog* wizard = new ArduinoProjectWizardDialog(this, parent);
    wizard->setPath(parameters.defaultPath());

    for (QWizardPage *p : wizard->extensionPages())
        wizard->addPage(p);

    return wizard;
}

GeneratedFiles ArduinoProjectWizard::generateFiles(const QWizard *w, QString *errorMessage) const
{
    const ArduinoProjectWizardDialog *dialog = qobject_cast<const ArduinoProjectWizardDialog*>(w);
    //const QtProjectParameters projectParams = dialog->projectParameters();
    const QString projectName = dialog->projectName();
    const QString projectPath = dialog->path() + "/" + projectName;
    const QString mainSourceFileName = buildFileName(projectPath, projectName, ".ino");

    Core::GeneratedFile mainSource(mainSourceFileName);
    setInoFileContent(mainSource, projectName + ".ino");

    Core::GeneratedFiles rc;
    rc << mainSource;

    return rc;
}

void ArduinoProjectWizard::setInoFileContent(Core::GeneratedFile &file, const QString &fileName)
{
    QFile inoFile(QLatin1String(":/wizards/arduino.ino"));
    inoFile.open(QIODevice::ReadOnly);

    QByteArray fileData = inoFile.readAll();
    QString content = QString::fromLatin1(fileData.data(), fileData.size());
    content.replace("%FILENAME%", fileName);

    file.setContents(content);
}

} // namespace Internal
} // namespace Arduino

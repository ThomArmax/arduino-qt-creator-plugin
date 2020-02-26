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
#include "avrprojectwizard.h"
#include "../../../arduinoconstants.h"

#include <utils/fileutils.h>

namespace Arduino {
namespace Internal {

using namespace Core;

AvrProjectWizard::AvrProjectWizard()
{
    setSupportedProjectTypes({Constants::ARDUINO_PROJECT_WIZARD_CATEGORY});
    setIcon(QIcon(QLatin1String(":/wizards/avr.png")));
    setDisplayName(tr("AVR C Project"));
    setId("Avr.Project.Makefile");
    setDescription(tr("Creates an AVR C project"));
    setCategory(QLatin1String(Constants::ARDUINO_PROJECT_WIZARD_CATEGORY));
    setDisplayCategory(QLatin1String(Constants::ARDIUNO_PROJECT_WIZARD_CATEGORY_DISPLAY));
    setFlags(Core::IWizardFactory::PlatformIndependent);
}

BaseFileWizard *AvrProjectWizard::create(QWidget *parent, const WizardDialogParameters &parameters) const
{

    return nullptr;
//    auto wizard = new SimpleProjectWizardDialog(this, parent);
//    wizard->setPath(parameters.defaultPath());

//    for (QWizardPage *p : wizard->extensionPages())
//        wizard->addPage(p);

//    return wizard;
}

GeneratedFiles AvrProjectWizard::generateFiles(const QWizard *w, QString *errorMessage) const
{
    GeneratedFile generatedProFile("dummy");
    return GeneratedFiles{generatedProFile};
}


} // namespace Internal
} // namespace Arduino

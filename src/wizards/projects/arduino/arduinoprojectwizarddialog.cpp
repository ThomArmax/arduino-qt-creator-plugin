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
#include "arduinoprojectwizarddialog.h"
#include "arduinohardwareconfigurationpage.h"
#include "../../../arduinoconstants.h"

#include <utils/fileutils.h>
#include <utils/filewizardpage.h>

#include <projectexplorer/kitinformation.h>
#include <projectexplorer/projectexplorer.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/targetsetuppage.h>
#include <qtsupport/qtkitinformation.h>
#include <qtsupport/qtsupportconstants.h>

using namespace Core;
using namespace Utils;

namespace Arduino {
namespace Internal {

ArduinoProjectWizardDialog::ArduinoProjectWizardDialog(const BaseFileWizardFactory *factory, QWidget *parent)
    : BaseFileWizard(factory, QVariantMap(), parent)
{
    m_firstPage = new FileWizardPage;
    m_firstPage->setTitle(tr("Project Name and Location"));
    m_firstPage->setFileNameLabel(tr("Project name:"));
    m_firstPage->setPathLabel(tr("Location:"));
    addPage(m_firstPage);

    m_secondPage = new ArduinoHardwareConfigurationPage(this);
    m_secondPage->setTitle(tr("Hardware configuration"));
    addPage(m_secondPage);

    m_targetSetupPage = new ProjectExplorer::TargetSetupPage;
    m_targetSetupPage->setUseScrollArea(false);
    //m_targetSetupPage->setRequiredKitPredicate(Core::Id(Constants::AVR_KIT_ID));
    m_targetSetupPage->initializePage();
    m_targetSetupPage->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    addPage(m_targetSetupPage);
}

QString ArduinoProjectWizardDialog::path() const
{
    return m_firstPage->path();
}

void ArduinoProjectWizardDialog::setPath(const QString &path)
{
    m_firstPage->setPath(path);
}

FileNameList ArduinoProjectWizardDialog::selectedFiles() const
{
//    return m_secondPage->selectedFiles();
}

FileNameList ArduinoProjectWizardDialog::selectedPaths() const
{
//    return m_secondPage->selectedPaths();
}

QString ArduinoProjectWizardDialog::projectName() const
{
    return m_firstPage->fileName();
}

} // namespace Internal
} // namespace Arduino

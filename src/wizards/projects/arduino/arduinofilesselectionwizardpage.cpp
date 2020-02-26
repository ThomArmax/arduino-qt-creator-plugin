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
#include "arduinofilesselectionwizardpage.h"
#include "arduinoprojectwizarddialog.h"

#include <projectexplorer/selectablefilesmodel.h>

namespace Arduino {
namespace Internal {

ArduinoFilesSelectionWizardPage::ArduinoFilesSelectionWizardPage(ArduinoProjectWizardDialog *arduinoProjectWizard, QWidget *parent)
    : QWizardPage(parent)
    , m_arduinoProjectWizardDialog(arduinoProjectWizard)
    , m_filesWidget(new ProjectExplorer::SelectableFilesWidget(this))
{

}

void ArduinoFilesSelectionWizardPage::initializePage()
{
    m_filesWidget->resetModel(Utils::FileName::fromString(m_arduinoProjectWizardDialog->path()),
                              Utils::FileNameList());
}

void ArduinoFilesSelectionWizardPage::cleanupPage()
{
    m_filesWidget->cancelParsing();
}

bool ArduinoFilesSelectionWizardPage::isComplete() const
{
    return m_filesWidget->hasFilesSelected();
}

Utils::FileNameList ArduinoFilesSelectionWizardPage::selectedPaths() const
{
    return m_filesWidget->selectedPaths();
}

Utils::FileNameList ArduinoFilesSelectionWizardPage::selectedFiles() const
{
    return m_filesWidget->selectedFiles();
}

} // namespace Internal
} // namespace Arduino

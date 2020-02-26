#include "arduinofilesselectionwizardpage.h"
#include "arduinoprojectwizarddialog.h"

#include <projectexplorer/selectablefilesmodel.h>

namespace Arduino {
namespace Internal {

ArduinoFilesSelectionWizardPage::ArduinoFilesSelectionWizardPage(ArduinoProjectWizardDialog *genericProjectWizard, QWidget *parent)
    : QWizardPage(parent)
    , m_genericProjectWizardDialog(genericProjectWizard)
    , m_filesWidget(new ProjectExplorer::SelectableFilesWidget(this))
{

}

void ArduinoFilesSelectionWizardPage::initializePage()
{
    m_filesWidget->resetModel(Utils::FileName::fromString(m_genericProjectWizardDialog->path()),
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

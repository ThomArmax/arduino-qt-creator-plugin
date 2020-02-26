#include "arduinoprojectwizarddialog.h"
#include "arduinofilesselectionwizardpage.h"

#include <utils/fileutils.h>
#include <utils/filewizardpage.h>

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

//    m_secondPage = new ArduinoFilesSelectionWizardPage(this);
//    m_secondPage->setTitle(tr("File Selection"));
//    addPage(m_secondPage);
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

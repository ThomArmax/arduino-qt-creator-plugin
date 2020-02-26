#ifndef ARDUINOPROJECTWIZARDDIALOG_H
#define ARDUINOPROJECTWIZARDDIALOG_H

#include <coreplugin/basefilewizard.h>
#include <utils/fileutils.h>
#include <utils/wizard.h>

using namespace Core;
using namespace Utils;

namespace Utils { class FileWizardPage; }

namespace Arduino {
namespace Internal {

class ArduinoFileSelectionWizardPage;

class ArduinoProjectWizardDialog : public BaseFileWizard
{
    Q_OBJECT

public:
    explicit ArduinoProjectWizardDialog(const BaseFileWizardFactory *factory, QWidget *parent);

    QString path() const;
    void setPath(const QString &path);
    FileNameList selectedFiles() const;
    FileNameList selectedPaths() const;
    QString projectName() const;

    FileWizardPage *m_firstPage;
//    ArduinoFileSelectionWizardPage *m_secondPage;
};


} // namespace Internal
} // namespace Arduino

#endif // ARDUINOPROJECTWIZARDDIALOG_H

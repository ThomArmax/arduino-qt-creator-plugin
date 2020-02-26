#ifndef ARDUINOFILESSELECTIONWIZARDPAGE_H
#define ARDUINOFILESSELECTIONWIZARDPAGE_H

#include <QWizardPage>
#include <utils/fileutils.h>

namespace ProjectExplorer { class SelectableFilesWidget; }

namespace Arduino {
namespace Internal {

class ArduinoProjectWizardDialog;

class ArduinoFilesSelectionWizardPage : public QWizardPage
{
public:
    explicit ArduinoFilesSelectionWizardPage(ArduinoProjectWizardDialog *genericProjectWizard, QWidget *parent = nullptr);

    bool isComplete() const override;
    void initializePage() override;
    void cleanupPage() override;
    Utils::FileNameList selectedFiles() const;
    Utils::FileNameList selectedPaths() const;

private:
    ArduinoProjectWizardDialog *m_genericProjectWizardDialog;
    ProjectExplorer::SelectableFilesWidget *m_filesWidget;
};

} // namespace Internal
} // namespace Arduino

#endif // ARDUINOFILESSELECTIONWIZARDPAGE_H

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

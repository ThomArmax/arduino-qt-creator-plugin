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
#ifndef ARDUINOPROJECT_H
#define ARDUINOPROJECT_H

#include <projectexplorer/project.h>
#include <projectexplorer/projectnodes.h>

#include <QElapsedTimer>
#include <QFutureWatcher>
#include <QTimer>

using namespace ProjectExplorer;

namespace Arduino {
namespace Internal {

class ArduinoProject : public Project
{
    Q_OBJECT

public:
    explicit ArduinoProject(const Utils::FileName &fileName);

    bool needsConfiguration() const override;
    bool supportsKit(ProjectExplorer::Kit *k, QString *errorMessage) const override;
    Utils::FileNameList nimFiles() const;
    QVariantMap toMap() const override;

    bool addFiles(const QStringList &filePaths);
    bool removeFiles(const QStringList &filePaths);
    bool renameFile(const QString &filePath, const QString &newFilePath);

protected:
    RestoreResult fromMap(const QVariantMap &map, QString *errorMessage) override;

private:
    void scheduleProjectScan();
    void collectProjectFiles();
    void updateProject();

    QStringList m_files;
    QStringList m_excludedFiles;
    QFutureWatcher<QList<ProjectExplorer::FileNode *>> m_futureWatcher;
    QElapsedTimer m_lastProjectScan;
    QTimer m_projectScanTimer;
};

} // namespace Internal
} // namespace Arduino

#endif // ARDUINOPROJECT_H

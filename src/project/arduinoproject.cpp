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
#include "arduinoproject.h"
#include "arduinoprojectnode.h"
#include "../arduinoconstants.h"

#include "../toolchain/arduinotoolchain.h"

#include <coreplugin/icontext.h>
#include <coreplugin/progressmanager/progressmanager.h>
#include <coreplugin/iversioncontrol.h>
#include <coreplugin/vcsmanager.h>
#include <projectexplorer/buildconfiguration.h>
#include <projectexplorer/kit.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/projectnodes.h>
#include <projectexplorer/target.h>
#include <projectexplorer/toolchain.h>
#include <projectexplorer/kitinformation.h>

#include <texteditor/textdocument.h>

#include <utils/algorithm.h>
#include <utils/qtcassert.h>
#include <utils/runextensions.h>

using namespace ProjectExplorer;
using namespace Utils;

namespace Arduino {
namespace Internal {


const int MIN_TIME_BETWEEN_PROJECT_SCANS = 4500;

ArduinoProject::ArduinoProject(const FileName &fileName)
    : Project(Constants::ARDUINO_INO_PROJECT_MIMETYPE, fileName)
{
    setId(Constants::ARDUINO_INO_PROJECT_ID);
    setDisplayName(fileName.toFileInfo().completeBaseName());
    setProjectLanguages(Core::Context(ProjectExplorer::Constants::CXX_LANGUAGE_ID));

    m_projectScanTimer.setSingleShot(true);
    connect(&m_projectScanTimer, &QTimer::timeout, this, &ArduinoProject::collectProjectFiles);

    connect(&m_futureWatcher, &QFutureWatcher<QList<FileNode *>>::finished, this, &ArduinoProject::updateProject);

    collectProjectFiles();
}

bool ArduinoProject::needsConfiguration() const
{
    return targets().empty();
}

void ArduinoProject::scheduleProjectScan()
{
    auto elapsedTime = m_lastProjectScan.elapsed();
    if (elapsedTime < MIN_TIME_BETWEEN_PROJECT_SCANS) {
        if (!m_projectScanTimer.isActive()) {
            m_projectScanTimer.setInterval(MIN_TIME_BETWEEN_PROJECT_SCANS - elapsedTime);
            m_projectScanTimer.start();
        }
    } else {
        collectProjectFiles();
    }
}

bool ArduinoProject::addFiles(const QStringList &filePaths)
{
    m_excludedFiles = Utils::filtered(m_excludedFiles, [&](const QString &f) { return !filePaths.contains(f); });
    scheduleProjectScan();
    return true;
}

bool ArduinoProject::removeFiles(const QStringList &filePaths)
{
    m_excludedFiles.append(filePaths);
    m_excludedFiles = Utils::filteredUnique(m_excludedFiles);
    scheduleProjectScan();
    return true;
}

bool ArduinoProject::renameFile(const QString &filePath, const QString &newFilePath)
{
    Q_UNUSED(filePath)
    m_excludedFiles.removeOne(newFilePath);
    scheduleProjectScan();
    return true;
}

void ArduinoProject::collectProjectFiles()
{
    m_lastProjectScan.start();
    QTC_ASSERT(!m_futureWatcher.future().isRunning(), return);
    FileName prjDir = projectDirectory();
    const QList<Core::IVersionControl *> versionControls = Core::VcsManager::versionControls();
    QFuture<QList<ProjectExplorer::FileNode *>> future = Utils::runAsync([prjDir, versionControls] {
        return FileNode::scanForFilesWithVersionControls(
                    prjDir, [](const FileName &fn) { return new FileNode(fn, FileType::Source, false); },
                    versionControls);
    });
    m_futureWatcher.setFuture(future);
    Core::ProgressManager::addTask(future, tr("Scanning for Nim files"), "Nim.Project.Scan");
}

void ArduinoProject::updateProject()
{
    emitParsingStarted();
    const QStringList oldFiles = m_files;
    m_files.clear();

    QList<FileNode *> fileNodes = Utils::filtered(m_futureWatcher.future().result(),
                                                  [&](const FileNode *fn) {
        const FileName path = fn->filePath();
        const QString fileName = path.fileName();
        const bool keep = !m_excludedFiles.contains(path.toString())
                && !fileName.endsWith(".ArduinoProject", HostOsInfo::fileNameCaseSensitivity())
                && !fileName.contains(".ArduinoProject.user", HostOsInfo::fileNameCaseSensitivity());
        if (!keep)
            delete fn;
        return keep;
    });

    m_files = Utils::transform(fileNodes, [](const FileNode *fn) { return fn->filePath().toString(); });
    Utils::sort(m_files, [](const QString &a, const QString &b) { return a < b; });

    if (oldFiles == m_files)
        return;

    auto newRoot = new ArduinoProjectNode(*this, projectDirectory());
    newRoot->setDisplayName(displayName());
    newRoot->addNestedNodes(fileNodes);
    setRootProjectNode(newRoot);
    emitParsingFinished(true);
}

bool ArduinoProject::supportsKit(Kit *k, QString *errorMessage) const
{
    auto tc = dynamic_cast<ArduinoToolChain*>(ToolChainKitInformation::toolChain(k, ProjectExplorer::Constants::CXX_LANGUAGE_ID));
    if (!tc) {
        if (errorMessage)
            *errorMessage = tr("No Arduino compiler set.");
        return false;
    }
    if (!tc->compilerCommand().exists()) {
        if (errorMessage)
            *errorMessage = tr("Arduino compiler does not exist.");
        return false;
    }
    return true;
}

FileNameList ArduinoProject::nimFiles() const
{
    const QStringList nim = files(AllFiles, [](const ProjectExplorer::Node *n) {
        return n->filePath().endsWith(".nim");
    });
    return Utils::transform(nim, [](const QString &fp) { return Utils::FileName::fromString(fp); });
}

QVariantMap ArduinoProject::toMap() const
{
    QVariantMap result = Project::toMap();
    //result[Constants::C_ArduinoProject_EXCLUDEDFILES] = m_excludedFiles;
    return result;
}

Project::RestoreResult ArduinoProject::fromMap(const QVariantMap &map, QString *errorMessage)
{
    //m_excludedFiles = map.value(Constants::C_ArduinoProject_EXCLUDEDFILES).toStringList();
    return Project::fromMap(map, errorMessage);
}


} // namespace Internal
} // namespace Arduino

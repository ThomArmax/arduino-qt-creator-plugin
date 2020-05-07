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
#include "arduinoprojectnode.h"
#include "arduinoproject.h"

using namespace ProjectExplorer;
using namespace Utils;

namespace Arduino {
namespace Internal {

ArduinoProjectNode::ArduinoProjectNode(ArduinoProject &project, const FileName &projectFilePath)
    : ProjectNode(projectFilePath)
    , m_project(project)
{

}

bool ArduinoProjectNode::supportsAction(ProjectAction action, const Node *node) const
{
    // TODO
    Q_UNUSED(action);
    Q_UNUSED(node);
    return false;
//    switch (node->nodeType()) {
//    case NodeType::File:
//        return action == ProjectAction::Rename
//            || action == ProjectAction::RemoveFile;
//    case NodeType::Folder:
//    case NodeType::Project:
//        return action == ProjectAction::AddNewFile
//            || action == ProjectAction::RemoveFile
//            || action == ProjectAction::AddExistingFile;
//    default:
//        return ProjectNode::supportsAction(action, node);
//    }
}

bool ArduinoProjectNode::addFiles(const QStringList &filePaths, QStringList *)
{
    return m_project.addFiles(filePaths);
}

bool ArduinoProjectNode::removeFiles(const QStringList &filePaths, QStringList *)
{
    return m_project.removeFiles(filePaths);
}

bool ArduinoProjectNode::deleteFiles(const QStringList &)
{
    return true;
}

bool ArduinoProjectNode::renameFile(const QString &filePath, const QString &newFilePath)
{
    return m_project.renameFile(filePath, newFilePath);
}

} // namespace Internal
} // namespace Arduino

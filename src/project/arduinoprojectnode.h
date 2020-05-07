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
#ifndef ARDUINOPROJECTNODE_H
#define ARDUINOPROJECTNODE_H

#include <projectexplorer/projectnodes.h>

namespace Arduino {
namespace Internal {

class ArduinoProject;

class ArduinoProjectNode : public ProjectExplorer::ProjectNode
{
public:
    ArduinoProjectNode(ArduinoProject &project, const Utils::FileName &projectFilePath);

    bool supportsAction(ProjectExplorer::ProjectAction action, const Node *node) const override;
    bool addFiles(const QStringList &filePaths, QStringList *) override;
    bool removeFiles(const QStringList &filePaths, QStringList *) override;
    bool deleteFiles(const QStringList &) override;
    bool renameFile(const QString &filePath, const QString &newFilePath) override;

private:
    ArduinoProject &m_project;
};

} // namespace Internal
} // namespace Arduino

#endif // ARDUINOPROJECTNODE_H

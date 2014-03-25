/*
  Copyright (C) 2013-2014 Stefano Verzegnassi

    This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License 3 as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

    This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
  along with this program. If not, see http://www.gnu.org/licenses/.
*/

#include "utils.h"

Utils::Utils(QObject *parent):
        QObject(parent)
{
    //
}

/************************************************************************************************************
* A collection of functions which can be useful for operations on files.			            *
*************************************************************************************************************/
QString Utils::getHomePath()
{
    return QDir::homePath();
}

bool Utils::checkFolderExists(QString path)
{
    return QDir(path).exists();
}

void Utils::mkPath(QString path)
{
    QDir dir;
    // FIXME: setPath is a dirty workaround that doesn't work in Windows. (e.g. I want to create a folder in D:\Documents - it returns "C:\D:\Documents")
    dir.setPath(QDir::rootPath());
    dir.mkpath(path);
}

bool Utils::checkFileExists(QString path)
{
    QFile fi(path);
    return fi.exists();
}

QString Utils::getFileBaseNameFromPath(QString path)
{
    QFileInfo fi(path);
    QString base = fi.baseName();

    return base;
}

QString Utils::getFileCompleteSuffixFromPath(QString path)
{
    QFileInfo fi(path);
    QString completeSuffix = fi.completeSuffix();

    // It does not include the first dot
    return completeSuffix;
}

void Utils::renameFile(QString completePathFile, QString newBaseName)
{
    QFileInfo pathfile(completePathFile);
    QString newPath = pathfile.canonicalPath() + QDir::separator() + newBaseName;
    if (!pathfile.completeSuffix().isEmpty()) {
        newPath += "." + pathfile.completeSuffix();
    }

    QFile file(completePathFile);
    file.rename(newPath);
}


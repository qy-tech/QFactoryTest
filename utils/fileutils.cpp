#include "fileutils.h"

#include <QFile>
#include <QIODevice>
#include <QStorageInfo>
#include <QTextStream>
#include <QtDebug>
#include <QtGlobal>
#ifdef Q_OS_WIN
#include <windows.h>
#endif
FileUtils::FileUtils(QObject* parent)
    : QObject { parent }
{
}

QString FileUtils::readFile(QString path)
{
    QFile file(path);
    if (!file.exists() || !file.isReadable()) {
        return "";
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return "";
    }
    QTextStream in(&file);
    auto result = in.readAll();
    file.close();
    return result;
}

bool FileUtils::writeFile(QString path, QString value)
{
    QFile file(path);
    if (!file.exists() || !file.isWritable()) {
        return false;
    }
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    QTextStream out(&file);
    out << value;
    return true;
}

bool FileUtils::appendFile(QString path, QString value)
{
    QFile file(path);
    if (!file.exists() || !file.isWritable()) {
        return false;
    }
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        return false;
    }
    QTextStream out(&file);
    out << value;
    return true;
}

QStringList FileUtils::getUdiskPaths()
{
    auto storages = QStorageInfo::mountedVolumes();
    QStringList usbLists;

    foreach (auto storage, storages) {
        qDebug() << "storage" << storage << "\n";
        if (storage.isValid() && storage.isReady()) {
#ifdef Q_OS_LINUX
            if (storage.rootPath().contains("/media/"))
#endif
#ifdef Q_OS_WIN
                UINT ret = GetDriveType((WCHAR*)storage.rootPath().utf16());
            if (ret == DRIVE_REMOVABLE)

#endif
            {
                usbLists.append(storage.rootPath());
                qDebug() << "storage" << storage << "\n";
            }
        }
    }
    return usbLists;
}

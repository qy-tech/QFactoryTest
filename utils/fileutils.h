#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QObject>

class FileUtils : public QObject {
    Q_OBJECT
public:
    explicit FileUtils(QObject* parent = nullptr);
    static QString readFile(QString path);
    static bool writeFile(QString path, QString value);
    static bool appendFile(QString path, QString value);
    static QStringList getUdiskPaths();

signals:
};

#endif // FILEUTILS_H

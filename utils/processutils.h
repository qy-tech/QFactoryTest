#ifndef PROCESSUTILS_H
#define PROCESSUTILS_H

#include <QMetaType>
#include <QObject>
struct ProcessResult {
    QString output;
    QString error;
    bool isSuccess;
};

class ProcessUtils : public QObject {
    Q_OBJECT
public:
    explicit ProcessUtils(QObject* parent = nullptr);
    static ProcessResult exec(QString command);
signals:
};

#endif // PROCESSUTILS_H

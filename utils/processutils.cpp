#include "processutils.h"

#include <QProcess>
#include <QtDebug>
ProcessUtils::ProcessUtils(QObject* parent)
    : QObject(parent)
{
}

ProcessResult ProcessUtils::exec(QString command)
{
    QProcess process;
    ProcessResult result;
    process.start("bash", QStringList() << "-c" << command);
    process.start();
    process.waitForFinished();

    result.error = process.readAllStandardError().trimmed();
    result.output = process.readAllStandardOutput().trimmed();
    result.isSuccess = !result.output.isEmpty();
    qDebug() << "output: " << result.output << "\n"
             << "error: " << result.error << "\n"
             << "isSuccess: " << result.isSuccess << "\n";
    process.close();

    return result;
}

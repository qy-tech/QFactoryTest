#include "udisktest.h"
#include "utils/fileutils.h"
#include <QDateTime>
#include <QStorageInfo>
#include <QtDebug>
UdiskTest::UdiskTest(QObject* parent)
    : BaseTestCase(parent)
{
    timestamp = QString::number(QDateTime::currentMSecsSinceEpoch());
    qDebug() << "testMesage" << timestamp;
}

bool UdiskTest::writeTestFile(QString path)
{
    QFile testFile(path);
    if (!testFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    QTextStream out(&testFile);
    out << timestamp;
    testFile.close();
    return true;
}

bool UdiskTest::readTestFile(QString path)
{
    QFile testFile(path);
    if (!testFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }
    QTextStream in(&testFile);
    QString value = in.readAll();
    qDebug() << "timestamp: " << timestamp << "\n"
             << "read value: " << value << "\n";
    testFile.close();
    return timestamp.compare(value) == 0;
}

void UdiskTest::run()
{
    auto udiskPaths = FileUtils::getUdiskPaths();
    if (udiskPaths.isEmpty()) {
        description = tr("No available Udisk");
        return;
    }

    foreach (auto path, udiskPaths) {
        auto testPath = QString("%1/%2").arg(path, timestamp);
        bool writeResult = writeTestFile(testPath);
        bool readResult = readTestFile(testPath);
        qDebug() << "test file name " << testPath << "\n"
                 << "read " << readResult << "\n"
                 << "write " << writeResult << "\n";
        if (writeResult && readResult) {
            description.append(QString("%1 %2\t").arg(path, tr("ReadWrite")));
            isSuccess = true;
        } else {
            description.append(QString("%1 %2\t").arg(path, tr("unavailable")));
            isSuccess = false;
        }
        QFile testFile(testPath);
        if (testFile.exists()) {
            testFile.remove();
        }
    }
}

#include "batteryreadthread.h"

#include <QByteArray>
#include <QDebug>
#include <QFile>
#include <QThread>
BatteryReadThread::BatteryReadThread(QObject* parent)
    : QThread(parent)
{
}

void BatteryReadThread::run()
{
    while (isRunning()) {
        emit batteryChanged(updateBatteryInfo());
        QThread::msleep(1000);
    }
}

QByteArray BatteryReadThread::updateBatteryInfo()
{
    QByteArray content = nullptr;
    QFile file(batteryPath);
    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        content = file.readAll();
    }
    qDebug() << "batter info is" << content;

    file.close();
    return content;
}

void BatteryReadThread::setBatteryPath(QString path)
{
    batteryPath = path;
}

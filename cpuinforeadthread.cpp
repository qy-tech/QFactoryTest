#include "cpuinforeadthread.h"

#include <QByteArray>
#include <QDebug>
#include <QFile>
#include <QThread>

CpuInfoReadThread::CpuInfoReadThread(QObject* parent) : QThread(parent) {}

void CpuInfoReadThread::run() {
  while (isRunning()) {
    emit updateCpuInfo(readFile(temperaturePath), readFile(cpuFreqPath));
    QThread::msleep(1000);
  }
}

float CpuInfoReadThread::readFile(QString path) {
  QByteArray content = nullptr;
  QFile file(path);
  if (file.exists() && file.open(QIODevice::ReadOnly)) {
    content = file.readAll();
  }
  //    qDebug() << "readFile info is %s" << content;

  file.close();
  if (!content.isEmpty()) {
    return content.replace("\n", "").toInt() / 1000.0f;
  }
  return 0.0f;
}

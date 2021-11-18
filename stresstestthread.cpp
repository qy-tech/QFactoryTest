#include "stresstestthread.h"

#include "factorytestutils.h"
StressTestThread::StressTestThread(QObject* parent) : QThread(parent) {}

void StressTestThread::run() {
  while (isRunning()) {
    FactoryTestUtils::runCommand("stressapptest -M 128 -s 4680 -m 1");
    QThread::msleep(3000);
    emit stressTestCompleted();
    terminate();
  }
}

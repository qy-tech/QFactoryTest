#ifndef STRESSTESTTHREAD_H
#define STRESSTESTTHREAD_H
#include <QThread>

class StressTestThread : public QThread {
  Q_OBJECT
 public:
  StressTestThread(QObject* parent = 0);

 protected:
  void run() override;
 signals:
  void stressTestCompleted();
};

#endif  // STRESSTESTTHREAD_H

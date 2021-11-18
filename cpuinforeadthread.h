#ifndef CPUINFOREADTHREAD_H
#define CPUINFOREADTHREAD_H

#include <QThread>

class CpuInfoReadThread : public QThread {
  Q_OBJECT
 public:
  CpuInfoReadThread(QObject* parent = 0);
  virtual void run() override;

 private:
  QString temperaturePath = tr("/sys/class/thermal/thermal_zone0/temp");
  QString cpuFreqPath =
      tr("/sys/devices/system/cpu/cpufreq/policy0/scaling_cur_freq");
  float readFile(QString path);

 signals:
  void updateCpuInfo(float temperature, float cpuFreq);
};

#endif  // CPUINFOREADTHREAD_H

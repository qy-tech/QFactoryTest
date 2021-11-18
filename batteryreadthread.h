#ifndef BATTERYREADTHREAD_H
#define BATTERYREADTHREAD_H

#include <QThread>

class BatteryReadThread : public QThread {
    Q_OBJECT
public:
    BatteryReadThread(QObject* parent = 0);
    virtual void run() override;
    void setBatteryPath(QString path);

private:
    QString batteryPath = tr("/sys/class/power_supply/rk-bat/capacity");
    QByteArray updateBatteryInfo();

signals:
    void batteryChanged(QString);
};

#endif // BATTERYREADTHREAD_H

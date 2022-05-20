#ifndef WIFITEST_H
#define WIFITEST_H

#include "basetestcase.h"
#include "utils/processutils.h"
#include <QObject>

class WifiTest : public BaseTestCase {
    Q_OBJECT
public:
    explicit WifiTest(QObject* parent = nullptr);
    explicit WifiTest(QObject* parent = nullptr, QString ssid = "", QString passwd = "");

    ProcessResult installModule();
    ProcessResult removeModule();
    ProcessResult connectWifi();
    ProcessResult reconnectWifi();
    ProcessResult disconnectWifi();
    ProcessResult ifconfigWlan();
    ProcessResult getWifiStatus();
    ProcessResult getWifiSignalLevel();

    const QString& getSsid() const;
    void setSsid(const QString& newSsid);

    const QString& getPasswd() const;
    void setPasswd(const QString& newPasswd);

public slots:
    void run();

private:
    QString ssid;
    QString passwd;
signals:
};

#endif // WIFITEST_H

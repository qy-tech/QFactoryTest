#include "wifitest.h"

#include <QThread>
#include <QTime>
#include <QtDebug>
#include <utils/processutils.h>

WifiTest::WifiTest(QObject* parent)
    : BaseTestCase(parent)
{
}

WifiTest::WifiTest(QObject* parent, QString ssid, QString passwd)
    : BaseTestCase(parent)
{
    this->passwd = passwd;
    this->ssid = ssid;
}

const QString& WifiTest::getSsid() const
{
    return ssid;
}

void WifiTest::setSsid(const QString& newSsid)
{
    ssid = newSsid;
}

const QString& WifiTest::getPasswd() const
{
    return passwd;
}

void WifiTest::setPasswd(const QString& newPasswd)
{
    passwd = newPasswd;
}

void WifiTest::run()
{
    if (ssid.isEmpty()) {
        description = tr("Identification SSID failed");
        return;
    }
    auto result = ifconfigWlan();
    if (!result.isSuccess) {
        description = tr("No detection network card");
        installModule();
    }
    QTime dieTime = QTime::currentTime().addMSecs(TIMER_TIMEOUT);
    while (QTime::currentTime() < dieTime && isRunning()) {
        result = getWifiStatus();

        if (!result.isSuccess) { // 获取WiFi状态失败，直接链接WiFi
            description = tr("Failure to get wifi information");
            connectWifi();
            msleep(3000);
        } else if (!result.output.contains("wpa_state=COMPLETED")) { // wpa_state 没有链接成功
            reconnectWifi();
            description = tr("WiFi connection fails");
        } else if (result.output.contains("ip_address=")
            || result.output.contains("ssid=")) { // wpa_state 链接成功并获取到IP地址
            result = getWifiSignalLevel();
            auto wifiInfo = result.output.trimmed().split("\t");
            if (wifiInfo.size() > 2) {
                description = QString("%1: %2dBm").arg(tr("signal level"), wifiInfo[2]);
            }
            isSuccess = true;
            break;
        }
        msleep(1000);
    }
}

ProcessResult WifiTest::installModule()
{
    return ProcessUtils::exec("insmod /system/lib/modules/8723cs.ko");
}

ProcessResult WifiTest::removeModule()
{
    return ProcessUtils::exec("rmmod /system/lib/modules/8723cs.ko");
}

ProcessResult WifiTest::connectWifi()
{
    return ProcessUtils::exec(QString("wifi_start.sh %1 %2").arg(ssid, passwd));
}

ProcessResult WifiTest::reconnectWifi()
{
    return ProcessUtils::exec("wpa_cli -i wlan0 -p /var/run/wpa_supplicant reconnect");
}

ProcessResult WifiTest::ifconfigWlan()
{
    return ProcessUtils::exec("ifconfig wlan0");
}

ProcessResult WifiTest::getWifiStatus()
{
    return ProcessUtils::exec("wpa_cli -i wlan0 -p /var/run/wpa_supplicant status");
}

ProcessResult WifiTest::getWifiSignalLevel()
{
    return ProcessUtils::exec("wpa_cli -i wlan0 -p /var/run/wpa_supplicant scan_result");
}

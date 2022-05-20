#include "bttest.h"
#include <QTime>
#include <QtDebug>
BtTest::BtTest(QObject* parent)
    : BaseTestCase(parent)
{
}

ProcessResult BtTest::initBt()
{
    return ProcessUtils::exec("/usr/bin/bt_init.sh");
}

ProcessResult BtTest::getHciConfig()
{
    return ProcessUtils::exec("hciconfig -a");
}

ProcessResult BtTest::getBtDevice()
{
    return ProcessUtils::exec("hcitool dev");
}

ProcessResult BtTest::upBtDevice()
{
    return ProcessUtils::exec("hciconfig hci0 up");
}

ProcessResult BtTest::downBtDevice()
{
    return ProcessUtils::exec("hciconfig hci0 down");
}

void BtTest::run()
{
    qDebug() << "running bt test";
    QTime dieTime = QTime::currentTime().addMSecs(TIMER_TIMEOUT);
    while (QTime::currentTime() < dieTime && isRunning()) {
        auto result = getHciConfig();
        if (!result.isSuccess) {
            description = tr("No Bluetooth device was detected");
            initBt();
            msleep(3000);
        } else if (result.output.contains("Name:")) {
            result = getBtDevice();
            auto btDev = result.output.trimmed().split("\t");
            if (btDev.size() > 2) {
                description = QString("%1: %2").arg(tr("address"), btDev[2]);
            }
            isSuccess = true;
            break;
        } else {
            description = tr("Bluetooth is closed");
            upBtDevice();
        }
        msleep(1000);
    }
}

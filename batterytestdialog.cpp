#include "batterytestdialog.h"
#include "batteryreadthread.h"
#include "factorytestutils.h"
#include "ui_batterytestdialog.h"

#include <QByteArray>
#include <QFile>
#include <QString>
#include <QThread>

BatteryTestDialog::BatteryTestDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::BatteryTestDialog)
{
    ui->setupUi(this);
    FactoryTestUtils::windowFullScreen(this);
    FactoryTestUtils::moveWidgetRightBottom(ui->buttonBox);
    thread = new BatteryReadThread();
    thread->setBatteryPath("/sys/class/power_supply/battery/capacity");
    thread->start();
    //接收读取的值
    connect(thread, SIGNAL(batteryChanged(QString)), this, SLOT(updateBatteryInfo(QString)));
}

BatteryTestDialog::~BatteryTestDialog()
{
    delete ui;
}

void BatteryTestDialog::closeEvent(QCloseEvent*)
{
    qDebug("close battery dialog");

    thread->terminate();
}

void BatteryTestDialog::accept()
{
    qDebug("close battery accept");
    thread->terminate();
    close();
}

void BatteryTestDialog::reject()
{
    qDebug("close battery reject");
    thread->terminate();
    close();
}

void BatteryTestDialog::updateBatteryInfo(QString batteryInfo)
{

    if (batteryInfo.isEmpty()) {
        ui->label_battery->setText(tr("ERROR"));
    } else {
        ui->label_battery->setText(QString("%1%").arg(batteryInfo.replace("\n", "").replace(" ", "")));
    }
}

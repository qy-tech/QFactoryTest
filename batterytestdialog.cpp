#include "batterytestdialog.h"

#include <QByteArray>
#include <QFile>
#include <QFont>
#include <QString>
#include <QThread>

#include "batteryreadthread.h"
#include "factorytestutils.h"
#include "ui_batterytestdialog.h"

BatteryTestDialog::BatteryTestDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::BatteryTestDialog) {
  ui->setupUi(this);
  FactoryTestUtils::windowFullScreen(this);
  FactoryTestUtils::moveWidgetRightBottom(ui->buttonBox);
  QFont ft;
  ft.setPointSize(25);
  ui->label_battery->setFont(ft);
  thread = new BatteryReadThread();
  thread->setBatteryPath("/sys/class/power_supply/rk-bat/capacity");
  thread->start();
  //接收读取的值
  connect(thread, SIGNAL(batteryChanged(QString)), this,
          SLOT(updateBatteryInfo(QString)));
}

BatteryTestDialog::~BatteryTestDialog() { delete ui; }

void BatteryTestDialog::closeEvent(QCloseEvent*) {
  qDebug("close battery dialog");

  thread->terminate();
}

void BatteryTestDialog::accept() {
  qDebug("close battery accept");
  thread->terminate();
  close();
  done(1);
}

void BatteryTestDialog::reject() {
  qDebug("close battery reject");
  thread->terminate();
  close();
  done(0);
}

void BatteryTestDialog::updateBatteryInfo(QString batteryInfo) {
  if (batteryInfo.isEmpty()) {
    ui->label_battery->setText(tr("获取电量失败"));
  } else {
    ui->label_battery->setText(
        QString("%1%").arg(batteryInfo.replace("\n", "").replace(" ", "")));
  }
}

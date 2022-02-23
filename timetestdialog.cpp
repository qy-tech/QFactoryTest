#include "timetestdialog.h"

#include <QDateTime>

#include "factorytestutils.h"
#include "ui_timetestdialog.h"

TimeTestDialog::TimeTestDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::TimeTestDialog) {
  ui->setupUi(this);

  FactoryTestUtils::windowFullScreen(this);

  FactoryTestUtils::runCommand("date +%Y%m%d+%T -s \"20211231  23:59:59\"");

  FactoryTestUtils::runCommand("hwclock -w");
  startTimer(1000);
}

TimeTestDialog::~TimeTestDialog() { delete ui; }

void TimeTestDialog::timerEvent(QTimerEvent* event) {
  QString time =
      QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd HH:mm:ss");

  ui->label->setText(
      QString(
          "将系统默认时间设置为：2021-12-31 23:59:59\n获取到系统当前时间为：%1")
          .arg(time));
}

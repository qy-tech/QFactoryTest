#include "printertestdialog.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <QDebug>
#include <QKeyEvent>
#include <QMessageBox>

#include "factorytestutils.h"
#include "ui_printertestdialog.h"

PrinterTestDialog::PrinterTestDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::PrinterTestDialog) {
  ui->setupUi(this);
  FactoryTestUtils::windowFullScreen(this);
  // FactoryTestUtils::moveWidgetRightBottom(ui->buttonBox);
  ui->statusLabel->hide();

  th_print = new PrintThread();
  connect(th_print, SIGNAL(nopaperSig()), this, SLOT(nopaperSlot()));
  connect(th_print, SIGNAL(errorSig()), this, SLOT(errorSlot()));
  connect(th_print, SIGNAL(finishedSig()), this, SLOT(finishedSlot()));
}

PrinterTestDialog::~PrinterTestDialog() { delete ui; }

void PrinterTestDialog::on_btnPrinter_clicked() {
  if (!printRunningFlag) {
    qDebug() << "start print";
    ui->statusLabel->hide();
    ui->btnPrinter->setEnabled(false);
    th_print->start();
    printRunningFlag = true;
  } else {
    qDebug() << "is printing...";
  }
}

void PrinterTestDialog::on_okButton_clicked() {
  if (printTestResult) {
    accept();
  } else {
    reject();
  }
}

void PrinterTestDialog::on_failedButton_clicked() { reject(); }

void PrinterTestDialog::nopaperSlot() {
  qDebug() << "nopaperSlot";
  ui->statusLabel->show();
  printTestResult = false;
}

void PrinterTestDialog::errorSlot() {
  qDebug() << "errorSlot";
  printRunningFlag = false;
  ui->btnPrinter->setEnabled(true);
  printTestResult = false;
}

void PrinterTestDialog::finishedSlot() {
  qDebug() << "finishedSlot";
  printRunningFlag = false;
  ui->btnPrinter->setEnabled(true);
  printTestResult = true;
}

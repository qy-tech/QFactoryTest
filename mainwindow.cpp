#include "mainwindow.h"

#include "agingtestdialog.h"
#include "batterytestdialog.h"
#include "cameratestdialog.h"
#include "factorytestutils.h"
#include "keyboardtestdialog.h"
#include "lcdtestdialog.h"
#include "printertestdialog.h"
#include "timetestdialog.h"
#include "tptestdialog.h"
#include "ui_mainwindow.h"

#define FEATURE_STATE_FILE "/oem/feature_test_ok"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  showFullScreen();
  resize(FactoryTestUtils::screenWidth, FactoryTestUtils::screenHeight);
}

MainWindow::~MainWindow() {
  delete ui;
  // delete dialog;
}

void MainWindow::on_btn_keyboard_test_clicked() {
  KeyboardTestDialog dialog;
  int i = dialog.exec();
  qDebug("KeyboardTestDialog result = %d", i);
  setWidgetColor(ui->btn_keyboard_test, i);
  if (i == 1) {
    keyResult = true;
    checkAllTestResult();
  }
}
void MainWindow::on_btn_tp_test_clicked() {
  TPTestDialog dialog;
  int i = dialog.exec();
  qDebug("TPTestDialog result = %d", i);
  setWidgetColor(ui->btn_tp_test, i);
  if (i == 1) {
    tpResult = true;
    checkAllTestResult();
  }
}

void MainWindow::on_btn_lcd_test_clicked() {
  LcdTestDialog dialog;
  int i = dialog.exec();
  qDebug("LcdTestDialog result = %d", i);
  setWidgetColor(ui->btn_lcd_test, i);
  if (i == 1) {
    lcdResult = true;
    checkAllTestResult();
  }
}

void MainWindow::on_btn_camera_test_clicked() {
  CameraTestDialog dialog;
  int i = dialog.exec();
  qDebug("CameraTestDialog result = %d", i);
  setWidgetColor(ui->btn_camera_test, i);
  if (i == 1) {
    cameraResult = true;
    checkAllTestResult();
  }
}

void MainWindow::on_btn_printer_test_clicked() {
  PrinterTestDialog dialog;
  int i = dialog.exec();
  qDebug("PrinterTest result = %d", i);
  setWidgetColor(ui->btn_printer_test, i);
  if (i == 1) {
    printResult = true;
    checkAllTestResult();
  }
}

void MainWindow::on_btn_battery_test_clicked() {
  BatteryTestDialog dialog;
  int i = dialog.exec();
  qDebug("BatteryTestDialog result = %d", i);
  setWidgetColor(ui->btn_battery_test, i);
  if (i == 1) {
    batteryResult = true;
    checkAllTestResult();
  }
}

void MainWindow::on_btn_rtc_test_clicked() {
  TimeTestDialog dialog;
  int result = dialog.exec();
  qDebug() << "rtc test result " << result;
  setWidgetColor(ui->btn_rtc_test, result);
  if (result == 1) {
    rtcResult = true;
    checkAllTestResult();
  }
}

void MainWindow::setWidgetColor(QPushButton* widget, int result) {
  QPalette pal = widget->palette();
  if (result == 1) {
    pal.setColor(QPalette::Button, Qt::green);
  } else {
    pal.setColor(QPalette::Button, Qt::red);
  }
  widget->setPalette(pal);
  widget->setAutoFillBackground(true);
  widget->setFlat(true);
}

void MainWindow::startAgingTest() {
  AgingTestDialog dialog;
  dialog.exec();
}

void MainWindow::checkAllTestResult() {
  if (tpResult && lcdResult && keyResult && batteryResult && cameraResult &&
      printResult && rtcResult) {
    qDebug("All item test OK..");
    QMessageBox messagebox;
    messagebox.setText(tr("全部测试成功!!!\n\n 是否进入老化测试"));
    messagebox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    int ret = messagebox.exec();
    if (ret == QMessageBox::Ok) {
      QFile file(FEATURE_STATE_FILE);
      if (file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        file.open(QIODevice::ReadWrite | QIODevice::Text);
        file.write("OK");
        file.close();
        system("sync");
      }
      startAgingTest();
    } else {
      qDebug("ret==QMessageBox::OK");
      system("sync");
      system("reboot");
    }
  } else {
    qDebug("have item test faild..");
  }
}

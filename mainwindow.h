#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QFile>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

 private slots:
  void on_btn_tp_test_clicked();

  void on_btn_lcd_test_clicked();

  void on_btn_keyboard_test_clicked();

  void on_btn_battery_test_clicked();

  void on_btn_camera_test_clicked();

  void on_btn_printer_test_clicked();

  void on_btn_rtc_test_clicked();

 private:
  Ui::MainWindow* ui;
  // QDialog* dialog;
  bool tpResult = false;
  bool lcdResult = false;
  bool keyResult = false;
  bool batteryResult = false;
  bool rtcResult = false;
  bool cameraResult = false;
  bool printResult = false;

  void checkAllTestResult();
  void startAgingTest();
  void setWidgetColor(QPushButton* widget, int result);
};
#endif  // MAINWINDOW_H

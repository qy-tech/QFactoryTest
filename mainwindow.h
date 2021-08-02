#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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



private:
    Ui::MainWindow* ui;
    QDialog* dialog;
};
#endif // MAINWINDOW_H

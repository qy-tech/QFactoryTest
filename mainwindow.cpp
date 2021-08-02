#include "mainwindow.h"

#include "batterytestdialog.h"
#include "cameratestdialog.h"
#include "factorytestutils.h"
#include "keyboardtestdialog.h"
#include "lcdtestdialog.h"
#include "printertestdialog.h"
#include "tptestdialog.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(FactoryTestUtils::screenWidth, FactoryTestUtils::screenHeight);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete dialog;
}

void MainWindow::on_btn_keyboard_test_clicked()
{
    dialog = new KeyboardTestDialog(this);
    dialog->show();
}
void MainWindow::on_btn_tp_test_clicked()
{
    dialog = new TPTestDialog(this);
    dialog->show();
}

void MainWindow::on_btn_lcd_test_clicked()
{
    dialog = new LcdTestDialog(this);
    dialog->show();
}

void MainWindow::on_btn_camera_test_clicked()
{
    dialog = new CameraTestDialog(this);
    dialog->show();
}

void MainWindow::on_btn_printer_test_clicked()
{
    dialog = new PrinterTestDialog(this);
    dialog->show();
}

void MainWindow::on_btn_battery_test_clicked()
{

    dialog = new BatteryTestDialog(this);
    dialog->show();
}

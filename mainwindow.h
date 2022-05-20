#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QTableWidgetItem>
#include <QtGlobal>
#include <testcase/basetestcase.h>
#define HEADER_COLUMN_NAME 0
#define HEADER_COLUMN_STATUS 1
#define HEADER_COLUMN_RESULT 2
#define HEADER_COLUMN_DESCRIPTION 3

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

protected:
    void closeEvent(QCloseEvent* event);

private:
    Ui::MainWindow* ui;
    QString scanConfigFile();
    void loadConfig();
    void initTestCase();
    void showSystemInfo();
    void autoTest();
    QSettings* settings;
    bool isAutoTest = false;
    QStringList testList;
    int currentIndex = -1;
    BaseTestCase* testcase;
    QString configPath;

public slots:
    void onItemClickListener(int row, int column);
    void onTestFinished();
    void onTestStarted();
};
#endif // MAINWINDOW_H

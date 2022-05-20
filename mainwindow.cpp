#include "mainwindow.h"
#include "testcase/bttest.h"
#include "testcase/udisktest.h"
#include "testcase/wifitest.h"
#include "ui_mainwindow.h"
#include "utils/fileutils.h"
#include "utils/systeminfoutils.h"
#include <QDir>
#include <QFormLayout>
#include <QHeaderView>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPair>
#include <QProcess>
#include <QSettings>
#include <QString>
#include <QtDebug>
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    showSystemInfo();
    configPath = scanConfigFile();
    if (!configPath.isEmpty()) {
        loadConfig();
        initTestCase();
        autoTest();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent* event)
{

    testcase->quit();
    isAutoTest = false;
    QMainWindow::closeEvent(event);
}

QString MainWindow::scanConfigFile()
{
    QStringList configPaths;
    configPaths << "/tmp";
    configPaths << FileUtils::getUdiskPaths();
    foreach (auto path, configPaths) {
        QString configPath = QString("%1/%2").arg(path, "Factory_Test.ini");
        QFile file(QDir::toNativeSeparators(configPath));
        if (file.exists()) {
            return configPath;
        }
    }
    return "";
}

void MainWindow::loadConfig()
{
    testList = QStringList();

    settings = new QSettings(configPath, QSettings::IniFormat);
    auto groups = settings->childGroups();
    foreach (auto group, groups) {
        settings->beginGroup(group);
        if (settings->value("activated") == "1") {
            qDebug() << "item " << group;
            testList.append(group);
        }
        settings->endGroup();
    }
}

void MainWindow::initTestCase()
{
    connect(ui->table_testcase, SIGNAL(cellClicked(int, int)), this,
        SLOT(onItemClickListener(int, int)));
    auto header = ui->table_testcase->horizontalHeader();
    auto headerStyle = QString("QHeaderView::section { border:0.5px solid #E5E5E5; }");
    header->setStyleSheet(headerStyle);
    header->setSectionResizeMode(header->count() - 1, QHeaderView::Stretch);
    ui->table_testcase->verticalHeader()->setVisible(false);
    ui->table_testcase->setShowGrid(true);
    ui->table_testcase->setRowCount(testList.size());
    for (int i = 0; i < testList.size(); i++) {
        ui->table_testcase->setItem(i, HEADER_COLUMN_NAME,
            new QTableWidgetItem(testList[i]));
        ui->table_testcase->setItem(
            i, HEADER_COLUMN_STATUS,
            new QTableWidgetItem(tr(BaseTestCase::STATUS_WATING)));
    }
}

void MainWindow::showSystemInfo()
{
    ui->table_device_info->setRowCount(2);
    ui->table_device_info->setColumnCount(8);
    ui->table_device_info->setSpan(0, 1, 1, 5);
    ui->table_device_info->setItem(0, 0, new QTableWidgetItem(tr("Version")));
    ui->table_device_info->setItem(0, 1, new QTableWidgetItem(SystemInfoUtils::getKernelVersion()));
    ui->table_device_info->setItem(0, 6, new QTableWidgetItem(tr("SerialNumber")));
    ui->table_device_info->setItem(0, 7, new QTableWidgetItem(SystemInfoUtils::getSerialNumber()));

    QList<QPair<QString, QString>> status;
    status.append(qMakePair(tr("Product"), SystemInfoUtils::getProduct()));
    status.append(qMakePair(tr("RAM"), SystemInfoUtils::getRamSpace()));
    status.append(qMakePair(tr("Flash"), SystemInfoUtils::getFlashSpace()));
    status.append(qMakePair(tr("MAC"), SystemInfoUtils::getMacAddress()));

    for (auto col = 0; col < status.size(); col++) {
        ui->table_device_info->setItem(1, 2 * col,
            new QTableWidgetItem(status[col].first));
        ui->table_device_info->setItem(1, 2 * col + 1,
            new QTableWidgetItem(status[col].second));
        ui->table_device_info->horizontalHeader()->setSectionResizeMode(2 * col, QHeaderView::Stretch);
        ui->table_device_info->horizontalHeader()->setSectionResizeMode(2 * col + 1, QHeaderView::Stretch);
    }
}

void MainWindow::autoTest()
{
    if (testList.isEmpty()) {
        return;
    }
    isAutoTest = true;
    currentIndex = 0;
    emit ui->table_testcase->cellClicked(currentIndex, 0);
}

void MainWindow::onItemClickListener(int row, int)
{
    QString testname = testList[row];
    qDebug() << QString("run %1 test").arg(testname);

    if ("WIFI" == testname) {
        settings->beginGroup(testname);
        QString ssid = settings->value("wifi_ap").toString();
        QString passwd = settings->value("password").toString();
        settings->endGroup();
        testcase = new WifiTest(this, ssid, passwd);
    } else if ("BT" == testname) {
        testcase = new BtTest(this);
    } else if ("USB" == testname) {
        testcase = new UdiskTest(this);
    }
    if (testcase != NULL) {
        qDebug() << "start test case";
        currentIndex = row;
        connect(testcase, SIGNAL(finished()), this, SLOT(onTestFinished()));
        connect(testcase, SIGNAL(started()), this, SLOT(onTestStarted()));

        testcase->start();
    }
    //    settings->endGroup();
}

void MainWindow::onTestStarted()
{
    ui->table_testcase->setEnabled(false);
    ui->table_testcase->setItem(
        currentIndex, HEADER_COLUMN_STATUS,
        new QTableWidgetItem(BaseTestCase::STATUS_TESTING));
}

void MainWindow::onTestFinished()
{
    qDebug() << "onTestFinished";

    ui->table_testcase->setEnabled(true);

    ui->table_testcase->setItem(
        currentIndex, HEADER_COLUMN_STATUS,
        new QTableWidgetItem(BaseTestCase::STATUS_FINISHED));

    qDebug() << "onTestResult isSuccess: " << testcase->isSuccess
             << " description: " << testcase->description;
    ui->table_testcase->setItem(
        currentIndex, HEADER_COLUMN_RESULT,
        new QTableWidgetItem(tr(testcase->isSuccess ? "PASS" : "FAIL")));
    ui->table_testcase->setItem(currentIndex, HEADER_COLUMN_DESCRIPTION,
        new QTableWidgetItem(testcase->description));
    for (int i = 0; i < ui->table_testcase->columnCount(); i++) {
        ui->table_testcase->item(currentIndex, i)
            ->setBackground(testcase->isSuccess ? QColor(100, 181, 246)
                                                : QColor(229, 115, 115));
    }
    ui->table_testcase->clearSelection();
    testcase->terminate();
    testcase = NULL;
    delete testcase;

    if (isAutoTest && currentIndex < ui->table_testcase->rowCount() - 1) {
        currentIndex++;
        emit ui->table_testcase->cellClicked(currentIndex, 0);
    } else {
        isAutoTest = false;
        currentIndex = 0;
    }
    qDebug() << "isAutoTest: " << isAutoTest
             << "\tcurrentIndex: " << currentIndex;
}

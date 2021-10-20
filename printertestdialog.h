#ifndef PRINTERTESTDIALOG_H
#define PRINTERTESTDIALOG_H

#include <QDialog>
#include "printthread.h"

#define PICW 672
#define PICH 384

#define CMD_IO_PAPER_DETECT_CTL _IOW('L', 0, unsigned long)
//#define CMD_IO_MOTORTEST _IOW('L', 1, unsigned long)
//#define CMD_IO_PRINT_LINE_TEST _IOW('L', 2, unsigned long)
#define CMD_IO_PRINT_START _IOW('L', 3, unsigned long)
#define CMD_IO_PRINT_STOP _IOW('L', 4, unsigned long)
#define CMD_IO_PRINT_PAPER_FEED _IOW('L', 5, unsigned long)
#define ENABLE_MEDIA 0

namespace Ui {
class PrinterTestDialog;
}

class PrinterTestDialog : public QDialog {
    Q_OBJECT

public:
    explicit PrinterTestDialog(QWidget* parent = nullptr);
    ~PrinterTestDialog();

private slots:
    void on_btnPrinter_clicked();
    void on_okButton_clicked();
    void on_failedButton_clicked();

public slots:
    void nopaperSlot();
    void errorSlot();
    void finishedSlot();

private:
    Ui::PrinterTestDialog* ui;

    QString printerPath = "/dev/printer_device_node";

    //bool printerTest();
    //bool toGray();

    float matriPicGlobal[PICH][PICW];
    float matriPicDealGlobal[PICH][PICW];
    float integralImageSumArr[PICW * PICH];
    float progressPicDeal;
    int qimageRGB[PICH][PICW][3];
    uint8_t dotBuffer[PICW][PICH];
    int fd;

    bool printRunningFlag=false;
    bool printTestResult=false;

    PrintThread *th_print;
};

#endif // PRINTERTESTDIALOG_H

#ifndef PRINTTHREAD_H
#define PRINTTHREAD_H

#include <QDebug>
#include <QImage>
#include <QThread>

#define PICW 672
#define PICH 384

#define CMD_IO_PAPER_DETECT_CTL _IOW('L', 0, unsigned long)
//#define CMD_IO_MOTORTEST _IOW('L', 1, unsigned long)
//#define CMD_IO_PRINT_LINE_TEST _IOW('L', 2, unsigned long)
#define CMD_IO_PRINT_START _IOW('L', 3, unsigned long)
#define CMD_IO_PRINT_STOP _IOW('L', 4, unsigned long)
#define CMD_IO_PRINT_PAPER_FEED _IOW('L', 5, unsigned long)
#define ENABLE_MEDIA 0

class PrintThread : public QThread {
  Q_OBJECT

 public:
  PrintThread(QObject* parent = nullptr);

 signals:
  void nopaperSig();
  void errorSig();
  void finishedSig();

 protected:
  void run();

 private:
  bool toGray();
  bool printerTest();
  QString printerPath = "/dev/printer_device_node";

  float matriPicGlobal[PICH][PICW];
  float matriPicDealGlobal[PICH][PICW];
  float integralImageSumArr[PICW * PICH];
  float progressPicDeal;
  int qimageRGB[PICH][PICW][3];
  uint8_t dotBuffer[PICW][PICH];
  int fd;

  bool printRunningFlag = false;
  bool printTestResult = false;
};

#endif  // PRINTTHREAD_H

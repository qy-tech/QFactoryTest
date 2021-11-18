#include "printthread.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

PrintThread::PrintThread(QObject* parent) : QThread(parent) {}

void PrintThread::run() {
  bool res = printerTest();
  if (!res) {
    emit errorSig();
  } else {
    emit finishedSig();
  }
}

bool PrintThread::printerTest() {
  qDebug() << "PrintThread::printerTest";

  int fd = ::open("/dev/printer_device_node", O_RDWR);

  if (!toGray()) {
    qDebug() << "toGray error";
    return false;
  }
  int height = PICH;
  int width = PICW;

  // https://blog.csdn.net/lien0906/article/details/37557717?utm_medium=distribute.pc_relevant_download.none-task-blog-2~default~BlogCommendFromBaidu~default-4.nonecase&dist_request_id=1328741.37637.16169923175864223&depth_1-utm_source=distribute.pc_relevant_download.none-task-blog-2~default~BlogCommendFromBaidu~default-4.nonecas
  int S = width >> 3;
  int T = 15;
  int i, j;
  long sum = 0;
  int count = 0;
  int index;
  int x1, y1, x2, y2;
  int s2 = S / 2;
  int ret = 0;

  // create the integral image
  for (i = 0; i < width; i++) {
    // reset this column sum
    sum = 0;
    for (j = 0; j < height; j++) {
      index = j * width + i;
      sum += matriPicDealGlobal[j][i];  // input[index];
      if (i == 0)
        integralImageSumArr[index] = sum;
      else
        integralImageSumArr[index] = integralImageSumArr[index - 1] + sum;
    }
  }
  // perform thresholding
  for (i = 0; i < width; i++) {
    for (j = 0; j < height; j++) {
      index = j * width + i;
      // set the SxS region
      x1 = i - s2;
      x2 = i + s2;
      y1 = j - s2;
      y2 = j + s2;
      // check the border
      if (x1 < 0) x1 = 0;
      if (x2 >= width) x2 = width - 1;
      if (y1 < 0) y1 = 0;
      if (y2 >= height) y2 = height - 1;
      count = (x2 - x1) * (y2 - y1);
      // I(x,y)=s(x2,y2)-s(x1,y2)-s(x2,y1)+s(x1,x1)
      sum = integralImageSumArr[y2 * width + x2] -
            integralImageSumArr[y1 * width + x2] -
            integralImageSumArr[y2 * width + x1] +
            integralImageSumArr[y1 * width + x1];
      if ((long)(matriPicDealGlobal[j][i] * count) <
          (long)(sum * (100 - T) / 100)) {
        matriPicDealGlobal[j][i] = 0;
        dotBuffer[i][j] = 1;
      } else {
        matriPicDealGlobal[j][i] = 255;
        dotBuffer[i][j] = 0;
      }
    }
  }

  if (fd < 0) {
    perror("open fail!\n");
    return false;
  }

  ret = ioctl(fd, CMD_IO_PAPER_DETECT_CTL);
  if (ret < 0) {
    printf("no paper!!\n");
    // ui->statusLabel->show();
    emit nopaperSig();
    return false;
  }

  ioctl(fd, CMD_IO_PRINT_START);
  usleep(10);
  for (i = 672; i > 0; i--) {
    write(fd, &dotBuffer[i][0], 384);
  }

  ioctl(fd, CMD_IO_PRINT_PAPER_FEED);
  ioctl(fd, CMD_IO_PRINT_STOP);
  usleep(10);
  return true;
}

/**
 * 将图片转化为灰度
 */
bool PrintThread::toGray() {
  qDebug() << "toGray";
  QString filename = QString(":/image/printer_test_img.jpeg");

  // QImage& image;
  QImage* image = new QImage;
  if (!(image->load(filename)))  //加载图像
  {
    // QMessageBox::information(this, tr("打开图像失败"), tr("打开图像失败!"));
    delete image;
    return false;
  }

  if (image->width() != PICW || image->height() != PICH) {
    qDebug() << "toGray size error image size : " << image->width() << "  "
             << image->height();
    return false;
  }
  qDebug() << "toGray image size : " << image->width() << "  "
           << image->height();

  uint8_t pixValT;
  // QRgb * line;
  for (int y = 0; y < image->height(); y++) {
    QRgb* line = (QRgb*)image->scanLine(y);
    for (int x = 0; x < image->width(); x++) {
      pixValT = qRed(line[x]) * 0.114 + qGreen(line[x]) * 0.587 +
                qBlue(line[x]) * 0.299;
      matriPicGlobal[y][x] = pixValT;

      qimageRGB[y][x][0] = qRed(line[x]);
      qimageRGB[y][x][1] = qGreen(line[x]);
      qimageRGB[y][x][2] = qBlue(line[x]);
    }
  }

  for (int y = 0; y < PICH; y++) {
    for (int x = 0; x < PICW; x++) {
      matriPicDealGlobal[y][x] = matriPicGlobal[y][x];
    }
  }

  return true;
}

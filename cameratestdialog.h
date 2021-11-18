#ifndef CAMERATESTDIALOG_H
#define CAMERATESTDIALOG_H

#include <QDialog>
extern "C" {
#include <assert.h>
#include <fcntl.h>
#include <getopt.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "rkmedia/rkmedia_api.h"
#include "rkmedia/rkmedia_venc.h"
#include "sample_common.h"
}

namespace Ui {
class CameraTestDialog;
}

class CameraTestDialog : public QDialog {
  Q_OBJECT

 public:
  explicit CameraTestDialog(QWidget* parent = nullptr);
  ~CameraTestDialog();

  static void video_packet_callback(MEDIA_BUFFER mb);

 protected:
  // virtual void closeEvent(QCloseEvent*) override;
  virtual void accept() override;
  virtual void reject() override;

  virtual bool event(QEvent* event) override;

 private:
  Ui::CameraTestDialog* ui;
  bool openCamera();
  void closeCamera();

  MPP_CHN_S stDestChn;
  MPP_CHN_S stSrcChn;
  RK_S32 s32CamId;
  // jpeg
  MPP_CHN_S stEncChn;
  VENC_RECV_PIC_PARAM_S stRecvParam;
  VENC_JPEG_PARAM_S stJpegParam;
};

#endif  // CAMERATESTDIALOG_H

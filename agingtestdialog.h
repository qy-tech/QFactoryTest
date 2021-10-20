#ifndef AGINGTESTDIALOG_H
#define AGINGTESTDIALOG_H

#include "cpuinforeadthread.h"
#include "stresstestthread.h"
#include "videoplayerthread.h"
#include <QDialog>
extern "C" {
#include "rkmedia/rkmedia_api.h"
#include "rkmedia/rkmedia_venc.h"
#include "sample_common.h"
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
}
namespace Ui {
class AgingTestDialog;
}

class AgingTestDialog : public QDialog {
    Q_OBJECT

public:
    explicit AgingTestDialog(QWidget* parent = nullptr);
    ~AgingTestDialog();
    static void video_packet_callback(MEDIA_BUFFER mb);

private:
    Ui::AgingTestDialog* ui;
    void playVideo();
    void toggleCamera();
    void stressapptest();
    void stopAgingTest();

    CpuInfoReadThread* cpuInfoReadThread;
    VideoPlayerThread* videoPlayerThread;
    StressTestThread* stressTestThread;

    QTimer* timer;
    QString currentTestCase;

    bool openCamera();
    void closeCamera();

    bool isCameraOpen = false;
    int startTestTime = 0;

    MPP_CHN_S stDestChn;
    MPP_CHN_S stSrcChn;
    RK_S32 s32CamId;
    //jpeg
    MPP_CHN_S stEncChn;
    VENC_RECV_PIC_PARAM_S stRecvParam;
    VENC_JPEG_PARAM_S stJpegParam;

protected:
    virtual void closeEvent(QCloseEvent*) override;
    virtual void accept() override;
    virtual void reject() override;

public slots:
    void updateCpuInfo(float temp, float cpuFreq);
    void onVideoPlayCompleted();
    void onCameraTimeOut();
    void onStressTestCompleted();
};

#endif // AGINGTESTDIALOG_H

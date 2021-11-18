#include "agingtestdialog.h"

#include <cameratestdialog.h>
#include <qeventloop.h>
#include <qprocess.h>
#include <qtimer.h>
#include <sys/time.h>

#include <QDateTime>
#include <QDebug>
#include <QMessageBox>
#include <QTimer>

#include "factorytestutils.h"
#include "ui_agingtestdialog.h"

AgingTestDialog::AgingTestDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::AgingTestDialog) {
  ui->setupUi(this);
  FactoryTestUtils::windowFullScreen(this);
  QFont font;
  font.setPointSize(32);
  ui->label_cpuInfo->setFont(font);

  totalTestTime = 0;
  toggleCamera();
  // playVideo();
  // stressapptest();
  cpuInfoReadThread = new CpuInfoReadThread();
  cpuInfoReadThread->start();
  connect(cpuInfoReadThread, SIGNAL(updateCpuInfo(float, float)), this,
          SLOT(updateCpuInfo(float, float)));
}

AgingTestDialog::~AgingTestDialog() { delete ui; }

/**
 * 视频播放老化测试
 */
void AgingTestDialog::playVideo() {
  // 视频老化测试
  // 记录开始测试的时间
  isAgingTestRunning = true;
  startTestTime = QDateTime::currentDateTime().toSecsSinceEpoch();
  currentTestCase = "正在执行视频播放测试";
  videoPlayerThread = new VideoPlayerThread();
  videoPlayerThread->start();
  connect(videoPlayerThread, SIGNAL(onPlayCompletedListener()), this,
          SLOT(onVideoPlayCompleted()));
}
/**
 * 摄像头开关老化测试
 */
void AgingTestDialog::toggleCamera() {
  // 开关摄像头测试
  // 记录开始测试的时间
  isAgingTestRunning = true;
  startTestTime = QDateTime::currentDateTime().toSecsSinceEpoch();
  currentTestCase = "正在执行摄像头开关测试";
  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(onCameraTimeOut()));
  timer->start(3000);
}

/**
 * 每隔 3 秒打开或者关闭一次摄像头
 */
void AgingTestDialog::onCameraTimeOut() {
  // 摄像头开关老化一个小时
  if (QDateTime::currentDateTime().toSecsSinceEpoch() - startTestTime >= 4680) {
    timer->stop();
    closeCamera();
    playVideo();
    return;
  }

  if (isCameraOpen) {
    closeCamera();
  } else {
    openCamera();
  }
}
/**
 * 打开摄像头
 */
bool AgingTestDialog::openCamera() {
  qDebug("vo_test0");
  isCameraOpen = true;
  s32CamId = 0;  // camid: camera ctx id, Default 0\n
  RK_BOOL bMultictx = RK_FALSE;
  char* iq_file_dir = NULL;
  if (iq_file_dir) {
    printf("#Rkaiq XML DirPath: %s\n", iq_file_dir);
    printf("#bMultictx: %d\n\n", bMultictx);
    rk_aiq_working_mode_t hdr_mode = RK_AIQ_WORKING_MODE_NORMAL;
    int fps = 30;
    SAMPLE_COMM_ISP_Init(s32CamId, hdr_mode, bMultictx, iq_file_dir);
    SAMPLE_COMM_ISP_Run(s32CamId);
    SAMPLE_COMM_ISP_SetFrameRate(s32CamId, fps);
  }
  printf("#CameraIdx: %d\n\n", s32CamId);

  int ret = 0;
  int video_width = 1920;
  int video_height = 1080;

  RK_MPI_SYS_Init();          //初始化系统
  VI_CHN_ATTR_S vi_chn_attr;  // VI 通道属性结构体指针。
  vi_chn_attr.pcVideoNode = "rkispp_scale0";  // video节点路径。
  vi_chn_attr.u32BufCnt = 3;
  vi_chn_attr.u32Width = video_width;            // video宽度。
  vi_chn_attr.u32Height = video_height;          // video高度。
  vi_chn_attr.enPixFmt = IMAGE_TYPE_NV12;        // video格式
  vi_chn_attr.enWorkMode = VI_WORK_MODE_NORMAL;  // VI通道工作模式
  ret = RK_MPI_VI_SetChnAttr(s32CamId, 0, &vi_chn_attr);  //设置VI通道属性。
  ret |= RK_MPI_VI_EnableChn(s32CamId, 0);                //启用VI通道
  if (ret) {
    printf("Create vi[0] failed! ret=%d\n", ret);
    return false;
  }

  // VO[0] for overlay plane, Only overlay plane support scale.
  VO_CHN_ATTR_S stVoAttr = {0};             //视频输出属性结构体。
  stVoAttr.pcDevNode = "/dev/dri/card0";    //视频输出设备节点
  stVoAttr.emPlaneType = VO_PLANE_OVERLAY;  //视频输出图层类型
  stVoAttr.enImgType = IMAGE_TYPE_NV12;     //视频输出格式。
  stVoAttr.u16Zpos = 0;                     //输出图层Z轴高度。
  stVoAttr.stImgRect.s32X = 0;  //输入图像尺寸参数。RECT_S s32X:区域的X轴坐标
  stVoAttr.stImgRect.s32Y = 0;                  //区域的Y轴坐标
  stVoAttr.stImgRect.u32Width = video_width;    //区域的宽度
  stVoAttr.stImgRect.u32Height = video_height;  //区域的高度
  stVoAttr.stDispRect.s32X =
      0;  //输出图层尺寸参数。用于vop裁剪。 RECT_S s32X:区域的X轴坐标
  stVoAttr.stDispRect.s32Y = 0;             //区域的Y轴坐标
  stVoAttr.stDispRect.u32Width = 800;       //区域的宽度
  stVoAttr.stDispRect.u32Height = 480;      //区域的高度
  ret = RK_MPI_VO_CreateChn(0, &stVoAttr);  //创建VO通道。
  if (ret) {
    printf("Create vo[0] failed! ret=%d\n", ret);
    return false;
  }

  // jpeg
  RK_U32 u32SrcWidth = 1920;
  RK_U32 u32SrcHeight = 1080;
  RK_U32 u32DstWidth = 1920;   // 1920 672
  RK_U32 u32DstHeight = 1080;  // 1080 384
  IMAGE_TYPE_E enPixFmt = IMAGE_TYPE_NV12;
  // const RK_CHAR *pcVideoNode = "rkispp_scale0";
  VENC_ROTATION_E enRotation =
      VENC_ROTATION_0;  // VENC_ROTATION_90 VENC_ROTATION_180

  VENC_CHN_ATTR_S venc_chn_attr;  // VENC通道属性结构体。
  memset(&venc_chn_attr, 0, sizeof(venc_chn_attr));
  venc_chn_attr.stVencAttr.enType =
      RK_CODEC_TYPE_JPEG;  // VENC_ATTR_S
                           // stVencAttr:编码器属性。enType:编码协议类型。
  venc_chn_attr.stVencAttr.imageType = enPixFmt;  // 输入图像类型
  venc_chn_attr.stVencAttr.u32PicWidth =
      u32SrcWidth;  //编码图像宽度。以像素为单位。
  venc_chn_attr.stVencAttr.u32PicHeight =
      u32SrcHeight;  //编码图像高度。以像素为单位。
  venc_chn_attr.stVencAttr.u32VirWidth =
      u32SrcWidth;  // stride宽度，通常与buffer_width相同。若u32VirWidth大于buffer宽度，则必须满足16对齐。
  venc_chn_attr.stVencAttr.u32VirHeight =
      u32SrcHeight;  // stride高度，通常与buffer_height相同。若u32VirHeight大于buffer高度，则必须满足16对齐。
  venc_chn_attr.stVencAttr.stAttrJpege.u32ZoomWidth =
      u32DstWidth;  //某种协议的编码器属性。这里使用stAttrJpege: attributes of
                    // jpeg.  u32ZoomWidth:缩放的指定宽度。
  venc_chn_attr.stVencAttr.stAttrJpege.u32ZoomHeight =
      u32DstHeight;  //缩放的指定高度。
  venc_chn_attr.stVencAttr.stAttrJpege.u32ZoomVirWidth =
      u32DstWidth;  //缩放的虚拟宽度。
  venc_chn_attr.stVencAttr.stAttrJpege.u32ZoomVirHeight =
      u32DstHeight;  //缩放的虚拟高度。
  venc_chn_attr.stVencAttr.enRotation = enRotation;
  ret = RK_MPI_VENC_CreateChn(0, &venc_chn_attr);  //创建编码通道。
  if (ret) {
    printf("Create Venc failed! ret=%d\n", ret);
  }

  ///*
  // MPP_CHN_S stEncChn;
  stEncChn.enModId = RK_ID_VENC;
  stEncChn.s32ChnId = 0;
  ret = RK_MPI_SYS_RegisterOutCb(
      &stEncChn, this->video_packet_callback);  //注册数据输出回调。
  if (ret) {
    printf("Register Output callback failed! ret=%d\n", ret);
    return false;
  }

  // The encoder defaults to continuously receiving frames from the previous
  // stage. Before performing the bind operation, set s32RecvPicNum to 0 to
  // make the encoding enter the pause state.
  // VENC_RECV_PIC_PARAM_S stRecvParam;
  stRecvParam.s32RecvPicNum = 0;
  RK_MPI_VENC_StartRecvFrame(0, &stRecvParam);

  // MPP_CHN_S stSrcChn;  MPP_CHN_S 定义模块设备通道结构体。
  stSrcChn.enModId = RK_ID_VI;  //模块号。
  stSrcChn.s32ChnId = 0;        //通道号。
  //    MPP_CHN_S stDestChn;
  //    stDestChn.enModId = RK_ID_VENC;
  //    stDestChn.s32ChnId = 0;
  printf("#Bind VI[0] to VENC[0]....\n");
  ret = RK_MPI_SYS_Bind(&stSrcChn, &stEncChn);  //数据源到数据接收者绑定接口。
  if (ret) {
    printf("Bind VI[1] to VENC[0]::JPEG failed! ret=%d\n", ret);
    return false;
  }

  printf("%s initial finish\n", __func__);

  // */

  printf("#Bind VI[0] to VO[0]....\n");
  stSrcChn.enModId = RK_ID_VI;
  stSrcChn.s32ChnId = 0;
  stDestChn.enModId = RK_ID_VO;
  stDestChn.s32ChnId = 0;
  ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
  if (ret) {
    printf("Bind vi[0] to vo[0] failed! ret=%d\n", ret);
    return false;
  }
  return true;
}

/**
 * 关闭摄像头
 */
void AgingTestDialog::closeCamera() {
  qDebug("closeCamera");
  isCameraOpen = false;
  int ret;
  stSrcChn.enModId = RK_ID_VI;
  stSrcChn.s32ChnId = 0;
  stDestChn.enModId = RK_ID_VO;
  stDestChn.s32ChnId = 0;
  ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
  if (ret) {
    printf("UnBind vi[0] to vo[0] failed! ret=%d\n", ret);
  }

  RK_MPI_VO_DestroyChn(0);

  ret = RK_MPI_SYS_UnBind(&stSrcChn, &stEncChn);
  if (ret) {
    printf("UnBind vi[0] to vo[0] failed! ret=%d\n", ret);
  }
  RK_MPI_VENC_DestroyChn(0);
  RK_MPI_VI_DisableChn(s32CamId, 0);
}

/**
 * 执行老化测试
 */
void AgingTestDialog::stressapptest() {
  isAgingTestRunning = true;
  currentTestCase = "正在执行压力测试";
  stressTestThread = new StressTestThread();
  stressTestThread->start();
  connect(stressTestThread, SIGNAL(stressTestCompleted()), this,
          SLOT(onStressTestCompleted()));
}

void AgingTestDialog::closeEvent(QCloseEvent*) {}

void AgingTestDialog::accept() {
  qDebug() << "AgingTestDialog accept";

  stopAgingTest();
  if (cpuInfoReadThread != NULL && cpuInfoReadThread->isRunning()) {
    cpuInfoReadThread->terminate();
    qDebug() << "stop cpuInfoReadThread";
  }
  if (videoPlayerThread != NULL && videoPlayerThread->isRunning()) {
    qDebug() << "stop videoPlayerThread";
    videoPlayerThread->terminate();
  }
  if (stressTestThread != NULL && stressTestThread->isRunning()) {
    qDebug() << "stop stressTestThread";
    stressTestThread->terminate();
  }
  close();
}

void AgingTestDialog::reject() {
  qDebug() << "AgingTestDialog reject";

  stopAgingTest();
  if (cpuInfoReadThread != NULL && cpuInfoReadThread->isRunning()) {
    cpuInfoReadThread->terminate();
    qDebug() << "stop cpuInfoReadThread";
  }

  if (videoPlayerThread != NULL && videoPlayerThread->isRunning()) {
    qDebug() << "stop videoPlayerThread";
    videoPlayerThread->terminate();
  }
  if (stressTestThread != NULL && stressTestThread->isRunning()) {
    qDebug() << "stop stressTestThread";
    stressTestThread->terminate();
  }
  close();
}

/**
 * 退出老化测试
 */
void AgingTestDialog::stopAgingTest() {
  qDebug() << "stopAgingTest";
  isAgingTestRunning = false;
  currentTestCase = "老化测试结束";
  QMessageBox messagebox;
  messagebox.setText(tr("老化测试结束，是否重启设备"));
  messagebox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
  int ret = messagebox.exec();
  if (ret == QMessageBox::Ok) {
    qDebug("ret==QMessageBox::OK");
    system("sync");
    system("reboot");
  }
}

/**
 * 刷新界面上的温度和 CPU 频率信息
 */
void AgingTestDialog::updateCpuInfo(float temp, float cpuFreq) {
  // qDebug() << "temp is" << temp << "cpu freq is " << cpuFreq;
  if (isAgingTestRunning) {
    totalTestTime += 1;
    ui->label_cpuInfo->setText(
        QString("%1\n温度：%2摄氏度\nCPU频率：%3Mhz\n已运行老化测试%4")
            .arg(currentTestCase)
            .arg(temp)
            .arg(cpuFreq)
            .arg(formatTime(totalTestTime * 1000)));

  } else {
    totalTestTime = 0;
    ui->label_cpuInfo->setText(QString("%1\n温度：%2摄氏度\nCPU频率：%3Mhz")
                                   .arg(currentTestCase)
                                   .arg(temp)
                                   .arg(cpuFreq));
  }
}

QString AgingTestDialog::formatTime(int ms) {
  int ss = 1000;
  int mi = ss * 60;
  int hh = mi * 60;
  int dd = hh * 24;

  long day = ms / dd;
  long hour = (ms - day * dd) / hh;
  long minute = (ms - day * dd - hour * hh) / mi;
  long second = (ms - day * dd - hour * hh - minute * mi) / ss;
  long milliSecond = ms - day * dd - hour * hh - minute * mi - second * ss;

  QString hou = QString::number(hour, 10);
  QString min = QString::number(minute, 10);
  QString sec = QString::number(second, 10);
  QString msec = QString::number(milliSecond, 10);

  // qDebug() << "minute:" << min << "second" << sec << "ms" << msec <<endl;

  return hou + "时" + min + "分" + sec + "秒";
}
/**
 * 每次视频播放完成的回调
 */
void AgingTestDialog::onVideoPlayCompleted() {
  // 播放视频一个小时
  if (QDateTime::currentDateTime().toSecsSinceEpoch() - startTestTime >= 4680) {
    if (videoPlayerThread != NULL) {
      videoPlayerThread->terminate();
    }

    stressapptest();
  }
}

/**
 * 压力测试完成回调
 */
void AgingTestDialog::onStressTestCompleted() {
  // 压力测试完成
  stopAgingTest();
}

/**
 * 摄像头数据回调
 */
void AgingTestDialog::video_packet_callback(MEDIA_BUFFER) {}

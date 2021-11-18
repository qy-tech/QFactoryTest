#include "cameratestdialog.h"

#include <RgaUtils.h>
#include <fcntl.h>
#include <malloc.h>
#include <rga.h>
#include <rga/im2d.h>
#include <rga/rga.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <QKeyEvent>

#include "factorytestutils.h"
#include "ui_cameratestdialog.h"

CameraTestDialog::CameraTestDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::CameraTestDialog) {
  ui->setupUi(this);
  FactoryTestUtils::windowFullScreen(this);
  ui->statuLabel->setVisible(false);
  // FactoryTestUtils::moveWidgetRightBottom(ui->buttonBox);
  if (!openCamera()) {
    ui->statuLabel->setVisible(true);
  }
}

CameraTestDialog::~CameraTestDialog() {
  // qDebug("~CameraTestDialog");
  // closeCamera();
  delete ui;
}

bool CameraTestDialog::openCamera() {
  qDebug("vo_test0");
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
  stVoAttr.stDispRect.u32Width = 640;       //区域的宽度
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

void CameraTestDialog::closeCamera() {
  qDebug("closeCamera");
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

bool CameraTestDialog::event(QEvent* event) {
  if (event->type() == QEvent::KeyPress) {
    QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
    if (keyEvent->key() == 0x10000B7) {
      qDebug("receive power key close camera");
      closeCamera();
    }
  }
  return false;
}
void CameraTestDialog::accept() {
  qDebug("CameraTestDialog accept");
  closeCamera();
  done(1);
}
void CameraTestDialog::reject() {
  qDebug("CameraTestDialog reject");
  closeCamera();
  done(0);
}
void CameraTestDialog::video_packet_callback(MEDIA_BUFFER mb) {
  //    static RK_U32 jpeg_id = 0;
  //    printf("Get JPEG packet[%d]:ptr:%p, fd:%d, size:%zu, mode:%d,
  //    channel:%d, "
  //           "timestamp:%lld\n",
  //        jpeg_id, RK_MPI_MB_GetPtr(mb), RK_MPI_MB_GetFD(mb),
  //        RK_MPI_MB_GetSize(mb), RK_MPI_MB_GetModeID(mb),
  //        RK_MPI_MB_GetChannelID(mb), RK_MPI_MB_GetTimestamp(mb));

  //    char jpeg_path[128];
  //    sprintf(jpeg_path, "/tmp/test_jpeg%d.jpeg", jpeg_id);
  //    printf("jpeg_path = %s\n", jpeg_path);
  //    Widget::photoPath = QString::fromUtf8(jpeg_path);
  //    FILE* file = fopen(jpeg_path, "w");
  //    if (file) {
  //        fwrite(RK_MPI_MB_GetPtr(mb), 1, RK_MPI_MB_GetSize(mb), file);
  //        fclose(file);
  //    }

  //    RK_MPI_MB_ReleaseBuffer(mb);
  //    jpeg_id++;
  //    emit MyPointer->capturedSignal(Widget::photoPath);
}

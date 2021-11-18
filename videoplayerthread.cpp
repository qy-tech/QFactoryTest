#include "videoplayerthread.h"

#include "factorytestutils.h"
VideoPlayerThread::VideoPlayerThread(QObject* parent) : QThread(parent) {}

void VideoPlayerThread::run() {
  while (isRunning()) {
    FactoryTestUtils::runCommand(
        "gst-play-1.0 /oem/oem_normal/SampleVideo_1280x720_5mb.mp4");
    emit onPlayCompletedListener();
    QThread::msleep(3000);
  }
}

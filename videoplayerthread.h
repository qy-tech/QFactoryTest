#ifndef VIDEOPLAYERTHREAD_H
#define VIDEOPLAYERTHREAD_H

#include <QThread>
class VideoPlayerThread : public QThread {
  Q_OBJECT
 public:
  VideoPlayerThread(QObject* parent = nullptr);

 protected:
  void run() override;

 signals:
  void onPlayCompletedListener();
};

#endif  // VIDEOPLAYERTHREAD_H

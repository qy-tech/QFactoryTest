#ifndef TIMETESTDIALOG_H
#define TIMETESTDIALOG_H

#include <QDialog>
#include <QTimerEvent>

namespace Ui {
class TimeTestDialog;
}

class TimeTestDialog : public QDialog {
  Q_OBJECT

 public:
  explicit TimeTestDialog(QWidget* parent = nullptr);
  ~TimeTestDialog();

 protected:
  void timerEvent(QTimerEvent* event);

 private:
  Ui::TimeTestDialog* ui;
};

#endif  // TIMETESTDIALOG_H

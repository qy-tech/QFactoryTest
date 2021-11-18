#include "keyboardtestdialog.h"

#include <QDebug>
#include <QKeyEvent>
#include <QLabel>
#include <QPainter>
#include <QPalette>
#include <QStyleOption>

#include "factorytestutils.h"
#include "ui_keyboardtestdialog.h"

KeyboardTestDialog::KeyboardTestDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::KeyboardTestDialog) {
  ui->setupUi(this);
  FactoryTestUtils::windowFullScreen(this);
  labelLists.append(ui->label_F1);
  labelLists.append(ui->label_F2);
  labelLists.append(ui->label_esc);
  labelLists.append(ui->label_power);
  changeKeyStatus(nullptr);
}

KeyboardTestDialog::~KeyboardTestDialog() { delete ui; }

void KeyboardTestDialog::changeKeyStatus(QLabel* label) {
  for (int i = 0; i < labelLists.size(); i++) {
    if (label == labelLists[i]) {
      labelLists[i]->setStyleSheet(styleSelect);
    } else {
      labelLists[i]->setStyleSheet(styleNormal);
    }
  }
}

void KeyboardTestDialog::keyReleaseEvent(QKeyEvent* event) {
  qDebug() << "keyReleaseEvent key " << event->text() << "code is "
           << event->key() << "\n";
  switch (event->key()) {
    case Qt::Key_F1:  // code 59 (KEY_F1)
      qDebug("key F1 select");
      changeKeyStatus(ui->label_F1);
      break;
    case Qt::Key_F2:  // code 60 (KEY_F2)
      qDebug("key F2 select");
      changeKeyStatus(ui->label_F2);
      break;
    case 0x10000B7:
      qDebug("key power select");
      changeKeyStatus(ui->label_power);
      break;
    case Qt::Key_Escape:  // code 1 (KEY_ESC)
      qDebug("key exit select");
      changeKeyStatus(ui->label_esc);
      break;
  }
}

bool KeyboardTestDialog::event(QEvent* event) {
  if (event->type() == QEvent::KeyPress) {
    QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
    qDebug() << "event press key " << keyEvent->text() << "code is "
             << keyEvent->key() << "\n";
    if (keyEvent->key() == Qt::Key_Escape) {
      qDebug("key Key_Escape select");
      changeKeyStatus(ui->label_esc);
      return true;
    } else {
      qDebug("key other");
    }
  }
  return QWidget::event(event);
}

void KeyboardTestDialog::on_okButton_clicked() { accept(); }

void KeyboardTestDialog::on_failedButton_clicked() { reject(); }

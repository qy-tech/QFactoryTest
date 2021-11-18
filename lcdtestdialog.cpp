#include "lcdtestdialog.h"

#include <QColor>
#include <QPalette>
#include <QVector>

#include "factorytestutils.h"
#include "ui_lcdtestdialog.h"
/**
 * 屏幕测试，在屏幕上显示纯色观察屏幕坏点
 */
LcdTestDialog::LcdTestDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::LcdTestDialog) {
  ui->setupUi(this);
  FactoryTestUtils::windowFullScreen(this);
  FactoryTestUtils::moveWidgetRightBottom(ui->buttonBox);

  initColorList();
}

LcdTestDialog::~LcdTestDialog() { delete ui; }

void LcdTestDialog::initColorList() {
  colorLists.append(Qt::white);
  colorLists.append(Qt::black);
  colorLists.append(Qt::red);
  colorLists.append(Qt::green);
  colorLists.append(Qt::blue);
  colorLists.append(Qt::yellow);
}

void LcdTestDialog::mouseReleaseEvent(QMouseEvent*) {
  qDebug("change screen color");

  QPalette palette(this->palette());
  palette.setColor(QPalette::Background, colorLists[currentColorIndex]);
  setAutoFillBackground(true);
  setPalette(palette);

  currentColorIndex++;
  if (currentColorIndex >= colorLists.size()) {
    currentColorIndex = 0;
  }
}

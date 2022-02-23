#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QFileInfo>

#include "agingtestdialog.h"
#include "factorytestutils.h"
#include "mainwindow.h"

#define FEATURE_STATE_FILE "/oem/feature_test_ok"

int FactoryTestUtils::screenWidth;
int FactoryTestUtils::screenHeight;
QString FactoryTestUtils::imagePath = "/data/test_camera.jpeg";

void updateScreenSize() {
  QDesktopWidget* desktop = QApplication::desktop();
  FactoryTestUtils::screenWidth = desktop->width();
  FactoryTestUtils::screenHeight = desktop->height();
}

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  updateScreenSize();

  QFileInfo file(FEATURE_STATE_FILE);

  if (file.exists() && file.isFile()) {
    printf("FEATURE_STATE_FILE exists\n");
    AgingTestDialog dialog;
    dialog.exec();
    return a.exec();
  }

  printf("FEATURE_STATE_FILE not exists\n");
  MainWindow w;
  w.show();

  return a.exec();
}

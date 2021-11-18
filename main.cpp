#include <QApplication>
#include <QDesktopWidget>

#include "factorytestutils.h"
#include "mainwindow.h"

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

  MainWindow w;
  w.show();
  return a.exec();
}

#ifndef FACTORYTESTUTILS_H
#define FACTORYTESTUTILS_H

#include <QString>
#include <QWidget>

class FactoryTestUtils {
 public:
  FactoryTestUtils();

  static void windowFullScreen(QWidget* widget);
  static void windowCenterScreen(QWidget* widget);
  static void moveWidgetRightBottom(QWidget* widget);

  static int screenWidth;
  static int screenHeight;
  static QString imagePath;
  static QString runCommand(QString cmd);
};

#endif  // FACTORYTESTUTILS_H

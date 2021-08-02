#include "factorytestutils.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QPalette>
#include <QSize>
#include <QWidget>
FactoryTestUtils::FactoryTestUtils()
{
}

void FactoryTestUtils::windowFullScreen(QWidget* widget)
{
    widget->showFullScreen();
}

void FactoryTestUtils::windowCenterScreen(QWidget* widget)
{

    widget->resize(screenWidth / 2, screenHeight / 2);
    widget->move((screenWidth - widget->width()) / 2, (screenHeight - widget->height()) / 2);
}

void FactoryTestUtils::moveWidgetRightBottom(QWidget* widget)
{
    widget->move(FactoryTestUtils::screenWidth * 0.95 - widget->width(),
        FactoryTestUtils::screenHeight * 0.95 - widget->height());
}

#include "tptestdialog.h"

#include <QDialogButtonBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>

#include "factorytestutils.h"
#include "ui_tptestdialog.h"
/**
* TP 测试，触摸屏幕会在屏幕上留下对应的轨迹
*/

TPTestDialog::TPTestDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::TPTestDialog)
{
    ui->setupUi(this);
    FactoryTestUtils::windowFullScreen(this);
    FactoryTestUtils::moveWidgetRightBottom(ui->buttonBox);


    pix = QPixmap(FactoryTestUtils::screenWidth, FactoryTestUtils::screenHeight);
    pix.fill(Qt::gray);
}

TPTestDialog::~TPTestDialog()
{
    delete ui;
}

void TPTestDialog::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QPainter pp(&pix);
    painter.drawPixmap(0, 0, pix);
    for (int i = 0; i < points.size() - 1; i++) {
        pp.drawLine(points[i], points[i + 1]);
    }
}

void TPTestDialog::mouseMoveEvent(QMouseEvent* event)
{
    QPoint pos = event->pos();
    points.append(pos);
    update();
};

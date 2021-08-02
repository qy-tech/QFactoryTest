#ifndef TPTESTDIALOG_H
#define TPTESTDIALOG_H

#include <QDialog>

namespace Ui {
class TPTestDialog;
}

class TPTestDialog : public QDialog {
    Q_OBJECT

public:
    explicit TPTestDialog(QWidget* parent = nullptr);
    ~TPTestDialog();

private:
    Ui::TPTestDialog* ui;
    QPixmap pix;
    QPoint lastPoint;
    QPoint endPoint;
    QVector<QPoint> points;

protected:
    virtual void paintEvent(QPaintEvent*);
    virtual void mouseMoveEvent(QMouseEvent*);
};

#endif // TPTESTDIALOG_H

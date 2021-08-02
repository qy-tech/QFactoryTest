#ifndef LCDTESTDIALOG_H
#define LCDTESTDIALOG_H

#include <QDialog>

namespace Ui {
class LcdTestDialog;
}

class LcdTestDialog : public QDialog {
    Q_OBJECT

public:
    explicit LcdTestDialog(QWidget* parent = nullptr);
    ~LcdTestDialog();

protected:
    virtual void mouseReleaseEvent(QMouseEvent*);

private:
    Ui::LcdTestDialog* ui;
    QVector<QColor> colorLists;
    void initColorList();
    int currentColorIndex = 0;
};

#endif // LCDTESTDIALOG_H

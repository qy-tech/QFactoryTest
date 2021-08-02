#ifndef KEYBOARDTESTDIALOG_H
#define KEYBOARDTESTDIALOG_H

#include <QDialog>
#include <QLabel>

namespace Ui {
class KeyboardTestDialog;
}

class KeyboardTestDialog : public QDialog {
    Q_OBJECT

public:
    explicit KeyboardTestDialog(QWidget* parent = nullptr);
    ~KeyboardTestDialog();

protected:
    virtual bool event(QEvent* event);

private:
    Ui::KeyboardTestDialog* ui;
    void changeKeyStatus(QLabel* label);
    QVector<QLabel*> labelLists;
    QString styleSelect = "background-color:green; border:none;";
    QString styleNormal = "background-color:gray; border:none;";
};

#endif // KEYBOARDTESTDIALOG_H

#ifndef BATTERYTESTDIALOG_H
#define BATTERYTESTDIALOG_H

#include "batteryreadthread.h"
#include <QDialog>
namespace Ui {
class BatteryTestDialog;
}

class BatteryTestDialog : public QDialog {
    Q_OBJECT

public:
    explicit BatteryTestDialog(QWidget* parent = nullptr);
    ~BatteryTestDialog();

private:
    Ui::BatteryTestDialog* ui;
    BatteryReadThread* thread;
public slots:
    void updateBatteryInfo(QString);

protected:
    virtual void closeEvent(QCloseEvent*) override;
    virtual void accept() override;
    virtual void reject() override;
};

#endif // BATTERYTESTDIALOG_H

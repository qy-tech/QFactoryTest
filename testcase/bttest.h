#ifndef BTTEST_H
#define BTTEST_H

#include "basetestcase.h"
#include "utils/processutils.h"
#include <QObject>

class BtTest : public BaseTestCase {
    Q_OBJECT
public:
    explicit BtTest(QObject* parent = nullptr);

private:
    ProcessResult initBt();
    ProcessResult getHciConfig();
    ProcessResult getBtDevice();
    ProcessResult upBtDevice();
    ProcessResult downBtDevice();

public slots:
    void run();
};

#endif // BTTEST_H

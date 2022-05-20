#ifndef UDISKTEST_H
#define UDISKTEST_H

#include "basetestcase.h"

#include <QFile>

class UdiskTest : public BaseTestCase {
public:
    explicit UdiskTest(QObject* parent = nullptr);

    bool writeTestFile(QString path);
    bool readTestFile(QString path);
    QString timestamp;

public slots:
    void run();
};

#endif // UDISKTEST_H

#ifndef BASETESTCASE_H
#define BASETESTCASE_H

#include <QObject>
#include <QThread>
#include <QTimer>
#define TIMER_TIMEOUT (20 * 1000)

class BaseTestCase : public QThread {
    Q_OBJECT
public:
    explicit BaseTestCase(QObject* parent = nullptr);

    bool isSuccess = false;
    QString description;
    static constexpr const char* STATUS_FINISHED = "finished";
    static constexpr const char* STATUS_TESTING = "testing";
    static constexpr const char* STATUS_WATING = "wating";

protected:
    QTimer* timer;
};

#endif // BASETESTCASE_H

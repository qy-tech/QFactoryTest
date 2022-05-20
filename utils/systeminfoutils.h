#ifndef SYSTEMINFOUTILS_H
#define SYSTEMINFOUTILS_H
#include <QObject>
#include <QtMath>

class SystemInfoUtils : public QObject {
    Q_OBJECT
public:
    SystemInfoUtils();
    static constexpr const char* FILENAME_PROC_VERSION = "/proc/version";
    static constexpr const char* FILENAME_MEMINFO = "/proc/meminfo";
    static constexpr const char* FILENAME_MAC = "/sys/class/net/*/address";
    static constexpr const char* UNKNOWN = "Unknown";
    // 获取Mac地址
    static QString getMacAddress();
    // 获取产品名称
    static QString getProduct();
    // 获取内存信息
    static QString getRamSpace();
    //获取硬盘信息
    static QString getFlashSpace();
    // 获取软件版本
    static QString getKernelVersion();
    // 获取序列号
    static QString getSerialNumber();

    static QString formatterSize(qint64 bytes);
};

#endif // SYSTEMINFOUTILS_H

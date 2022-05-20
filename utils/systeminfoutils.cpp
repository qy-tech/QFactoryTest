#include "systeminfoutils.h"
#include "utils/processutils.h"
#include <QLocale>
#include <QNetworkInterface>
#include <QStorageInfo>
#include <QtGlobal>
#include <QtMath>
#ifdef Q_OS_WIN
#include <windows.h>
#endif

SystemInfoUtils::SystemInfoUtils()
{
}

QString SystemInfoUtils::getMacAddress()
{
    foreach (QNetworkInterface netInterface, QNetworkInterface::allInterfaces()) {
        if (!(netInterface.flags() & QNetworkInterface::IsLoopBack))
            return netInterface.hardwareAddress();
    }
    return UNKNOWN;
}

QString SystemInfoUtils::getProduct()
{
    return QSysInfo::machineHostName();
}

QString SystemInfoUtils::getRamSpace()
{
    QString ram;
#ifdef Q_OS_MAC
    ProcessResult result = ProcessUtils::exec("sysctl hw.memsize | awk '/hw.memsize/ { print $2 }'");
    ram = formatterSize(result.output.toUInt());
#endif

#ifdef Q_OS_LINUX
    // /proc/meminfo 默认大小是kb 需要转化为bit
    ProcessResult result = ProcessUtils::exec("awk '/MemTotal/ { print $2 }' /proc/meminfo");
    ram = formatterSize(result.output.toUInt() * 1024);
#endif

#ifdef Q_OS_WIN
    // win
    MEMORYSTATUSEX memory_status;
    ZeroMemory(&memory_status, sizeof(MEMORYSTATUSEX));
    memory_status.dwLength = sizeof(MEMORYSTATUSEX);
    if (GlobalMemoryStatusEx(&memory_status)) {
        ram = formatterSize(memory_status.ullTotalPhys); /*QString("%1 MB").arg(qCeil(memory_status.ullTotalPhys / qPow(1024, 2)));*/
    } else {
        ram = UNKNOWN;
    }
#endif

    return ram;
}

QString SystemInfoUtils::getFlashSpace()
{
    // /proc/partitions 默认大小是kb 需要转化为bit
    ProcessResult result = ProcessUtils::exec("awk '/mmcblk.$/ {print $3}' /proc/partitions");
    if (result.isSuccess) {
        return formatterSize(result.output.toUInt() * 1024);
    }
    auto partitions = QStorageInfo::mountedVolumes();
    qint64 total = 0;
    foreach (auto partition, partitions) {
        if (partition.isReady()
            && (partition.device().contains("Volume") || partition.device().contains("/"))) {
            total += partition.bytesTotal();
        }
    }

    return formatterSize(total);
}

QString SystemInfoUtils::getKernelVersion()
{
    return QSysInfo::prettyProductName();
}

QString SystemInfoUtils::getSerialNumber()
{
    ProcessResult result = ProcessUtils::exec("awk '/Serial/ {print $3}' /proc/cpuinfo");
    return result.isSuccess ? result.output : UNKNOWN;
}

// 将bit转化为KB MB GB TB等
QString SystemInfoUtils::formatterSize(qint64 size)
{
    //    float num = size;
    //    QStringList list;
    //    list << "KB"
    //         << "MB"
    //         << "GB"
    //         << "TB";

    //    QStringListIterator i(list);
    //    QString unit("bytes");

    //    while (num >= 1024.0 && i.hasNext()) {
    //        unit = i.next();
    //        num /= 1024.0;
    //    }
    //    return QString().setNum(num, 'f', 2) + " " + unit;
    QLocale locale(QLocale::Chinese);
    return locale.formattedDataSize(size);
}

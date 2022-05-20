QT       += core gui network


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    testcase/basetestcase.cpp \
    testcase/bttest.cpp \
    testcase/udisktest.cpp \
    testcase/wifitest.cpp \
    utils/fileutils.cpp \
    utils/processutils.cpp \
    utils/systeminfoutils.cpp

HEADERS += \
    mainwindow.h \
    testcase/basetestcase.h \
    testcase/bttest.h \
    testcase/udisktest.h \
    testcase/wifitest.h \
    utils/fileutils.h \
    utils/processutils.h \
    utils/systeminfoutils.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    QFactoryTest_zh_CN.ts \
    QFactoryTest_en.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

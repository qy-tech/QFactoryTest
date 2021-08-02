QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    batteryreadthread.cpp \
    batterytestdialog.cpp \
    cameratestdialog.cpp \
    factorytestutils.cpp \
    keyboardtestdialog.cpp \
    lcdtestdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    printertestdialog.cpp \
    tptestdialog.cpp \
    sample_common_isp.c

HEADERS += \
    batteryreadthread.h \
    batterytestdialog.h \
    cameratestdialog.h \
    factorytestutils.h \
    keyboardtestdialog.h \
    lcdtestdialog.h \
    mainwindow.h \
    printertestdialog.h \
    tptestdialog.h \
    sample_common.h \
    rkmedia/rkmedia_adec.h \
    rkmedia/rkmedia_aenc.h \
    rkmedia/rkmedia_ai.h \
    rkmedia/rkmedia_ao.h \
    rkmedia/rkmedia_api.h \
    rkmedia/rkmedia_buffer.h \
    rkmedia/rkmedia_common.h \
    rkmedia/rkmedia_event.h \
    rkmedia/rkmedia_move_detection.h \
    rkmedia/rkmedia_muxer.h \
    rkmedia/rkmedia_occlusion_detection.h \
    rkmedia/rkmedia_rga.h \
    rkmedia/rkmedia_vdec.h \
    rkmedia/rkmedia_venc.h \
    rkmedia/rkmedia_vi.h \
    rkmedia/rkmedia_vmix.h \
    rkmedia/rkmedia_vo.h

FORMS += \
    batterytestdialog.ui \
    cameratestdialog.ui \
    keyboardtestdialog.ui \
    lcdtestdialog.ui \
    mainwindow.ui \
    printertestdialog.ui \
    tptestdialog.ui

INCLUDEPATH += ../../host/arm-buildroot-linux-gnueabihf/sysroot/usr/include/rkaiq/uAPI \
               ../../host/arm-buildroot-linux-gnueabihf/sysroot/usr/include/rkaiq/common \
               ../../host/arm-buildroot-linux-gnueabihf/sysroot/usr/include/rkaiq/xcore \
               ../../host/arm-buildroot-linux-gnueabihf/sysroot/usr/include/rkaiq/algos \
               ../../host/arm-buildroot-linux-gnueabihf/sysroot/usr/include/rkaiq/iq_parser \
               ../../host/arm-buildroot-linux-gnueabihf/sysroot/usr/include/rga \

LIBS+= -L ../../host/arm-buildroot-linux-gnueabihf/sysroot/usr/lib  -leasymedia  -lrockchip_mpp \
                        -lrga \
                        -lrkaiq \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    QtFactoryTest.qrc

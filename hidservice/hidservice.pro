QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
contains(ANDROID_TARGET_ARCH, arm64-v8a) {
    QT       += androidextras
}
contains(ANDROID_TARGET_ARCH, x86) {
    QT       += androidextras
}
CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    handler.cpp \
    main.cpp \
    mainwindow.cpp \
    response.cpp

HEADERS += \
    handler.h \
    mainwindow.h \
    response.h \
    router.hpp \

FORMS += \
    mainwindow.ui

include($$PWD/../common/common.pri)
include($$PWD/../json/json.pri)
unix {
    include($$PWD/../hidkit/unix.pri)
}

msvc {
    include($$PWD/../hidkit/win.pri)
}
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../hidkit/unix.pri

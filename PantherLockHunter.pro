QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

VERSION = 1.0.0
RC_ICONS = icons/app.ico
QMAKE_TARGET_COMPANY = "Med-Aid"
QMAKE_TARGET_PRODUCT = "Panther Lock Hunter"
QMAKE_TARGET_DESCRIPTION = "Panther Lock Hunter"
QMAKE_TARGET_COPYRIGHT = "Copyright \\251 2021"

#QMAKE_LFLAGS_WINDOWS += /MANIFESTUAC:"level='requireAdministrator'"

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

include(efsw/efsw.pri)
include(qtsingleapplication/qtsingleapplication.pri)

PRECOMPILED_HEADER = pch.h

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    pantherwatcher.cpp \
    pantherwatchlistener.cpp \
    scanlockfile.cpp \
    setting.cpp \
    systemtray.cpp \
    tababout.cpp \
    tablog.cpp \
    tabmain.cpp \
    tabsetting.cpp

HEADERS += \
    mainwindow.h \
    pantherwatcher.h \
    pantherwatchlistener.h \
    pch.h \
    scanlockfile.h \
    setting.h \
    systemtray.h \
    tababout.h \
    tablog.h \
    tabmain.h \
    tabsetting.h

FORMS += \
    mainwindow.ui \
    tababout.ui \
    tablog.ui \
    tabmain.ui \
    tabsetting.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

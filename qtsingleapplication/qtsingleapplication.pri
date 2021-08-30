QT += network widgets

INCLUDEPATH += $$PWD

SOURCES += \
        $$PWD/qtlockedfile.cpp \
        $$PWD/qtlocalpeer.cpp \
        $$PWD/qtsingleapplication.cpp \

unix: {
SOURCES += \
        $$PWD/qtlockedfile_unix.cpp \
}

win32: {
SOURCES += \
        $$PWD/qtlockedfile_win.cpp \
}

HEADERS += \
        $$PWD/qtlockedfile.h \
        $$PWD/qtlocalpeer.h \
        $$PWD/qtsingleapplication.h \
		

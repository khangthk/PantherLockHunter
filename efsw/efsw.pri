SOURCES += \
        $$PWD/Debug.cpp \
        $$PWD/DirWatcherGeneric.cpp \
        $$PWD/DirectorySnapshot.cpp \
        $$PWD/DirectorySnapshotDiff.cpp \
        $$PWD/FileInfo.cpp \
        $$PWD/FileSystem.cpp \
        $$PWD/FileWatcher.cpp \
        $$PWD/FileWatcherCWrapper.cpp \
        $$PWD/FileWatcherGeneric.cpp \
        $$PWD/FileWatcherImpl.cpp \
        $$PWD/Log.cpp \
        $$PWD/Mutex.cpp \
        $$PWD/String.cpp \
        $$PWD/System.cpp \
        $$PWD/Thread.cpp \
        $$PWD/Watcher.cpp \
        $$PWD/WatcherGeneric.cpp \

HEADERS += \
        $$PWD/Debug.hpp \
        $$PWD/DirWatcherGeneric.hpp \
        $$PWD/DirectorySnapshot.hpp \
        $$PWD/DirectorySnapshotDiff.hpp \
        $$PWD/FileInfo.hpp \
        $$PWD/FileSystem.hpp \
        $$PWD/FileWatcherGeneric.hpp \
        $$PWD/FileWatcherImpl.hpp \
        $$PWD/Lock.hpp \
        $$PWD/Mutex.hpp \
        $$PWD/String.hpp \
        $$PWD/System.hpp \
        $$PWD/Thread.hpp \
        $$PWD/Utf.hpp \
        $$PWD/Utf.inl \
        $$PWD/Watcher.hpp \
        $$PWD/WatcherGeneric.hpp \
        $$PWD/base.hpp \
        $$PWD/efsw.h \
        $$PWD/efsw.hpp \
        $$PWD/sophist.h \
        $$PWD/platform/platformimpl.hpp \

win32: {
SOURCES += \
        $$PWD/platform/win/FileSystemImpl.cpp \
        $$PWD/platform/win/MutexImpl.cpp \
        $$PWD/platform/win/SystemImpl.cpp \
        $$PWD/platform/win/ThreadImpl.cpp \
        $$PWD/FileWatcherWin32.cpp \
        $$PWD/WatcherWin32.cpp \

HEADERS += \
        $$PWD/platform/win/FileSystemImpl.hpp \
        $$PWD/platform/win/MutexImpl.hpp \
        $$PWD/platform/win/SystemImpl.hpp \
        $$PWD/platform/win/ThreadImpl.hpp \
        $$PWD/FileWatcherWin32.hpp \
        $$PWD/WatcherWin32.hpp \
} else {
SOURCES += \
        $$PWD/platform/posix/FileSystemImpl.cpp \
        $$PWD/platform/posix/MutexImpl.cpp \
        $$PWD/platform/posix/SystemImpl.cpp \
        $$PWD/platform/posix/ThreadImpl.cpp \

HEADERS += \
        $$PWD/platform/posix/FileSystemImpl.hpp \
        $$PWD/platform/posix/MutexImpl.hpp \
        $$PWD/platform/posix/SystemImpl.hpp \
        $$PWD/platform/posix/ThreadImpl.hpp \
}

unix:!macx: {
SOURCES += \
        $$PWD/FileWatcherInotify.cpp \
        $$PWD/WatcherInotify.cpp \

HEADERS += \
        $$PWD/FileWatcherInotify.hpp \
        $$PWD/WatcherInotify.hpp \
        $$PWD/inotify-nosys.h \
}

unix:macx: {
SOURCES += \
        $$PWD/FileWatcherFSEvents.cpp \
        $$PWD/FileWatcherKqueue.cpp \
        $$PWD/WatcherFSEvents.cpp \
        $$PWD/WatcherKqueue.cpp \

HEADERS += \
        $$PWD/FileWatcherFSEvents.hpp \
        $$PWD/FileWatcherKqueue.hpp \
        $$PWD/WatcherFSEvents.hpp \
        $$PWD/WatcherKqueue.hpp \
}

freebsd: {
SOURCES += \
        $$PWD/FileWatcherKqueue.cpp \
        $$PWD/WatcherKqueue.cpp \

HEADERS += \
        $$PWD/FileWatcherKqueue.hpp \
        $$PWD/WatcherKqueue.hpp \
}

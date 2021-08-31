#include "pantherwatchlistener.h"
#include "pantherwatcher.h"
#include "setting.h"

#include <QDebug>
#include <QFile>
#include <QString>
#include <QDir>
#include <QThread>

#include <efsw/FileSystem.hpp>
#include <efsw/System.hpp>

PantherWatchListener::PantherWatchListener(QObject *parent) : QObject(parent)
{
    initLockFileList();
    moveLockFileToTrash();
}

void PantherWatchListener::handleFileAction(efsw::WatchID watchID, const std::string &dir, const std::string &filename,
                                            efsw::Action action, std::string oldFilename)
{
    Q_UNUSED(watchID)
    Q_UNUSED(oldFilename)
    auto qdir = QString::fromStdString(dir);
    auto qfilename = QString::fromStdString(filename);
    if (m_lockFiles.contains(qfilename, Qt::CaseInsensitive) &&
       (action == efsw::Actions::Add || action == efsw::Actions::Modified)) {
        QString path = QDir::toNativeSeparators(qdir + QDir::separator()) + qfilename;
        QFile file(path);
        if (file.exists()) {
            bool success = true;
            if (m_moveToTrash) {
                if (file.moveToTrash() != success)
                {
                    QThread::msleep(20);
                    success = file.moveToTrash();
                }
            } else {
                if (file.remove() != success)
                {
                    QThread::msleep(20);
                    success = file.remove();
                }
            }

            emit fileDelete(path, success);
        }
    }
}

void PantherWatchListener::initLockFileList()
{
    m_lockFiles = Setting::getLockFiles();
}

void PantherWatchListener::moveLockFileToTrash()
{
    m_moveToTrash = Setting::getMoveToTrash();
}

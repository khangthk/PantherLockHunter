#include "pantherwatcher.h"
#include "pantherwatchlistener.h"

#include <QDebug>
#include <QFile>
#include <QString>

#include <efsw/FileSystem.hpp>
#include <efsw/System.hpp>

PantherWatcher::PantherWatcher(QObject *parent) : QObject(parent), m_watcher(nullptr), m_listener(nullptr)
{
    m_totalFileDeleted = 0;
}

PantherWatcher::~PantherWatcher()
{
    stop();
}

void PantherWatcher::start()
{
    m_watcher = new efsw::FileWatcher();
    m_listener = new PantherWatchListener(this);
    m_watcher->watch();

    connect(m_listener, &PantherWatchListener::fileDelete, this, &PantherWatcher::onFileDelete);
    connect(this, &PantherWatcher::updateLockFiles, m_listener, &PantherWatchListener::onUpdateLockFiles);
    connect(this, &PantherWatcher::moveToTrash, m_listener, &PantherWatchListener::onMoveToTrash);
}

void PantherWatcher::stop()
{
    efSAFE_DELETE(m_listener);
    efSAFE_DELETE(m_watcher);
}

bool PantherWatcher::addPath(const QString &path)
{
    if (isRunning() && m_watcher->addWatch(path.toStdString(), m_listener, true) > 0) {
        qDebug() << "Watching directory: " << path;
    } else {
        qDebug() << "Error trying to watch directory: " << path;
        qDebug() << efsw::Errors::Log::getLastErrorLog().c_str();
        return false;
    }

    return true;
}

bool PantherWatcher::removePath(const QString &path)
{
    if (isRunning()) {
        m_watcher->removeWatch(path.toStdString());
        return true;
    }

    return false;
}

bool PantherWatcher::isRunning() const
{
    return m_watcher != nullptr && m_listener != nullptr;
}

int PantherWatcher::getTotalFileDeleted()
{
    return m_totalFileDeleted;
}

void PantherWatcher::onFileDelete(const QString &path, const bool success)
{
    QString log = "Found: " + path;
    emit appendLog(log);
    qDebug() << log;
    log = success ? "->Deleted successfully!" : "->Delete failed!";
    emit appendLog(log);
    qDebug() << log;
    if (success) {
        ++m_totalFileDeleted;
        qDebug() << "Total file deleted: " << m_totalFileDeleted;
        emit totalFileDeleteChanged();
    }
}

void PantherWatcher::onUpdateLockFiles()
{
    emit updateLockFiles();
}

void PantherWatcher::onMoveToTrash()
{
    emit moveToTrash();
}

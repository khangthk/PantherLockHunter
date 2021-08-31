#ifndef PANTHERWATCHER_H
#define PANTHERWATCHER_H

#include <QObject>

#include <efsw/efsw.hpp>

class PantherWatchListener;

class PantherWatcher : public QObject
{
    Q_OBJECT
public:
    explicit PantherWatcher(QObject *parent = nullptr);
    virtual ~PantherWatcher();

    void start();
    void stop();
    bool addPath(const QString &path);
    bool removePath(const QString &path);
    bool isRunning() const;
    int getTotalFileDeleted();

signals:
    void addLog(const QString &log);
    void totalFileDeleteChanged();

public slots:
    void onFileDelete(const QString &path, const bool success);
    void onInitLockFileList();
    void onMoveLockFileToTrash();

private:
    efsw::FileWatcher *m_watcher;
    PantherWatchListener *m_listener;
    int m_totalFileDeleted;
};

#endif // PANTHERWATCHER_H

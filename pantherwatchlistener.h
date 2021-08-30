#ifndef PANTHERWATCHLISTENER_H
#define PANTHERWATCHLISTENER_H

#include <QObject>

#include <efsw/efsw.hpp>

class PantherWatchListener : public QObject, public efsw::FileWatchListener
{
    Q_OBJECT
public:
    explicit PantherWatchListener(QObject *parent = nullptr);
    void handleFileAction(efsw::WatchID watchID, const std::string &dir, const std::string &filename,
                          efsw::Action action, std::string oldFilename = "");

signals:
    void fileDelete(const QString &dir, const bool success);

public slots:
    void onUpdateLockFiles();
    void onMoveToTrash();

private:
    QStringList m_lockFiles;
    bool m_moveToTrash;
};

#endif // PANTHERWATCHLISTENER_H

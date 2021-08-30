#ifndef SETTING_H
#define SETTING_H

#include <QStringList>

class Setting
{
public:
    static QString getLastPath();
    static void setLastPath(const QString &path);

    static QStringList getWatchFolders();
    static void setWatchFolders(const QStringList &folders);

    static QStringList getLockFiles();
    static void setLockFiles(const QStringList &files);

    static QStringList getDefaultLockFiles();

    static bool getRunOnSystemStartup();
    static void setRunOnSystemStartup(const bool startup);

    static bool getHideWindowAtLaunch();
    static void setHideWindowAtLaunch(const bool hide);

    static bool getAutoStartHunter();
    static void setAutoStartHunter(const bool autoStart);

    static bool getMoveToTrash();
    static void setMoveToTrash(const bool toTrash);

    static bool getAutoClearLog();
    static void setAutoClearLog(const bool autoClear);

    static int getClearLogHours();
    static void setClearLogHours(const int hours);
};

#endif // SETTING_H

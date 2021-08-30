#include "setting.h"

#include <QCoreApplication>
#include <QDir>
#include <QSettings>

QString Setting::getLastPath()
{
    QSettings settings;
    return settings.value("LastPath", QVariant::fromValue(QDir::currentPath())).toString();
}

void Setting::setLastPath(const QString &path)
{
    QSettings settings;
    settings.setValue("LastPath", path);
    settings.sync();
}

QStringList Setting::getWatchFolders()
{
    QSettings settings;
    return settings.value("WatchFolders").toStringList();
}

void Setting::setWatchFolders(const QStringList &folders)
{
    QSettings settings;
    settings.setValue("WatchFolders", folders);
    settings.sync();
}

QStringList Setting::getLockFiles()
{
    QSettings settings;
    return settings.value("LockFiles", QVariant::fromValue(getDefaultLockFiles())).toStringList();
}

void Setting::setLockFiles(const QStringList &files)
{
    QSettings settings;
    settings.setValue("LockFiles", files);
    settings.sync();
}

QStringList Setting::getDefaultLockFiles()
{
    return { QString("!PatientLocked.dat"), QString("!MachineLocked.dat") };
}

bool Setting::getRunOnSystemStartup()
{
    QString regPath = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    QSettings settings(regPath, QSettings::NativeFormat);
    QString exePath = QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
    return exePath == settings.value("PantherLockHunter", "").toString();
}

void Setting::setRunOnSystemStartup(const bool startup)
{
    QString regPath = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    QSettings settings(regPath, QSettings::NativeFormat);
    if (startup) {
        QString exePath = QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
        settings.setValue("PantherLockHunter", exePath);
    } else {
        settings.remove("PantherLockHunter");
    }
}

bool Setting::getHideWindowAtLaunch()
{
    QSettings settings;
    return settings.value("HideWindowAtLaunch", false).toBool();
}

void Setting::setHideWindowAtLaunch(const bool hide)
{
    QSettings settings;
    settings.setValue("HideWindowAtLaunch", hide);
    settings.sync();
}

bool Setting::getAutoStartHunter()
{
    QSettings settings;
    return settings.value("AutoStartHunter", false).toBool();
}

void Setting::setAutoStartHunter(const bool autoStart)
{
    QSettings settings;
    settings.setValue("AutoStartHunter", autoStart);
    settings.sync();
}

bool Setting::getMoveToTrash()
{
    QSettings settings;
    return settings.value("MoveToTrash", false).toBool();
}

void Setting::setMoveToTrash(const bool toTrash)
{
    QSettings settings;
    settings.setValue("MoveToTrash", toTrash);
    settings.sync();
}

bool Setting::getAutoClearLog()
{
    QSettings settings;
    return settings.value("AutoClearLog", false).toBool();
}

void Setting::setAutoClearLog(const bool autoClear)
{
    QSettings settings;
    settings.setValue("AutoClearLog", autoClear);
    settings.sync();
}

int Setting::getClearLogHours()
{
    QSettings settings;
    return settings.value("ClearLogHours", 4).toInt();
}

void Setting::setClearLogHours(const int hours)
{
    QSettings settings;
    settings.setValue("ClearLogHours", hours);
    settings.sync();
}

#include "scanlockfile.h"
#include "setting.h"

ScanLockFile::ScanLockFile(QObject *parent) : QThread(parent),
    m_numberFilesDeleted(0), m_cancel(false)
{

}

void ScanLockFile::cancel()
{
    const QMutexLocker locker(&m_mutex);
    m_cancel = true;
}

bool ScanLockFile::deleteLockFile(const QString &filePath, const bool isMoveToTrash)
{
    QFile file(filePath);
    if (file.exists()) {
        bool success = true;
        if (isMoveToTrash) {
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
        return success;
    }
    return false;
}

bool ScanLockFile::isCanceled() const
{
    const QMutexLocker locker(&m_mutex);
    return m_cancel;
}

void ScanLockFile::run()
{
    m_numberFilesDeleted = 0;
    QStringList dirList = Setting::getWatchFolders();
    QStringList lockFileList = Setting::getLockFiles();
    bool isMoveToTrash = Setting::getMoveToTrash();

    QElapsedTimer timer;
    timer.start();

    QStringList details;
    foreach (auto &dir, dirList) {
        QDirIterator it(dir, lockFileList, QDir::Files | QDir::System | QDir::NoDotAndDotDot | QDir::NoSymLinks , QDirIterator::Subdirectories);
        while (!isCanceled() && it.hasNext()) {
            QString filePath = it.next();
            bool success = deleteLockFile(filePath, isMoveToTrash);
            success ? ++m_numberFilesDeleted : false;
            filePath = QDir::toNativeSeparators(filePath);
            qDebug() << "Found: " + filePath;
            details << "Found: " + filePath;
            qDebug() << (success ? "->Deleted successfully!" : "->Delete failed!");
            details << (success ? "->Deleted successfully!" : "->Delete failed!");
        }

        if (isCanceled()) {
            qDebug() << "Cancel scan lock file";
            break;
        }
    }

    qDebug() << "Scan done";
    emit scanDone(m_numberFilesDeleted, details);

    qDebug() << "Scan time: " << timer.elapsed() / 1000 << "s";
}

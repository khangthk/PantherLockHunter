#ifndef SCANLOCKFILE_H
#define SCANLOCKFILE_H

#include <QMutex>
#include <QThread>

class ScanLockFile : public QThread
{
    Q_OBJECT
public:
    explicit ScanLockFile(QObject *parent = nullptr);
    void cancel();

signals:
    void scanDone(const int numberFilesDeleted, const QStringList &details);

private:
    int m_numberFilesDeleted;
    mutable QMutex m_mutex; // protects m_cancel
    bool m_cancel;

    bool deleteLockFile(const QString &filePath, const bool isMoveToTrash);
    bool isCanceled() const;
    void run() override;
};

#endif // SCANLOCKFILE_H

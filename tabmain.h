#ifndef TABMAIN_H
#define TABMAIN_H

#include "scanlockfile.h"

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class TabMain; }
QT_END_NAMESPACE

class TabMain : public QWidget
{
    Q_OBJECT

public:
    explicit TabMain(QWidget *parent = nullptr);
    ~TabMain();

    bool scanning() const;

protected:
    void showEvent(QShowEvent *event) override;

signals:
    void addWatch(const QString &dir);
    void deleteWatch(const QStringList &listDir);
    void startWatcher();
    void numberItemOfListChanged(); // send this signal to update status bar green or yellow
    void startScan();
    void scanDone(const int numberFilesDeleted, const QStringList &details);

public slots:
    void onUpdateButtonStart(const bool isRunning);
    void onUpdateButtonScan();

private slots:
    void onAdd();
    void onDelete();
    void onStart();
    void onScan();
    void onListIndexChanged();
    void onScanLockFileDone(const int numberFilesDeleted, const QStringList &details);

private:
    Ui::TabMain *ui;
    bool m_isScanning;
    ScanLockFile *m_scanLockFile;

    void updateButtonDelete();
    void updateButtonScan();
};

#endif // TABMAIN_H

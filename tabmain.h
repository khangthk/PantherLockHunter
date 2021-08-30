#ifndef TABMAIN_H
#define TABMAIN_H

#include "pantherwatcher.h"

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

    PantherWatcher *getWatcher() const;

signals:
    void updateStatusBar(const bool running, const int numFileDeleted);
    void appendLog(const QString &log);
    void updateLockFiles();
    void moveToTrash();

public slots:
    void onTotalFileDeleteChanged();
    void onUpdateLockFiles();
    void onAutoStartHunter();
    void onUpdateStatusBar();
    void onMoveToTrash();

private slots:
    void onAdd();
    void onDelete();
    void onStart();
    void onScan();
    void onItemSelectionChanged();
    void onAppendLog(const QString &log);

protected:
    void showEvent(QShowEvent *event) override;

private:
    Ui::TabMain *ui;
    PantherWatcher *m_watcher;

    void updateButtonState();
    void getPantherSiteList(QStringList &list);
};

#endif // TABMAIN_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QTimer>

#include "tabmain.h"
#include "tablog.h"
#include "tabsetting.h"
#include "tababout.h"
#include "systemtray.h"
#include "pantherwatcher.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void starHunter(const bool autoStart);

protected:
    void showEvent(QShowEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

signals:
    void statusOfWatcherChanged(const bool isRunning);

public slots:
    void onSingleApplicationMessageReceived(const QString &message);

private slots:
    void onAddWatch(const QString &dir);
    void onDeleteWatch(const QStringList &listDir);
    void onStartWatcher();
    void onUpdateStatusBar();

private:
    Ui::MainWindow *ui;
    QLabel *m_statusIcon;
    QLabel *m_statusText;
    QLabel *m_totalFile;
    QTimer m_timer;
    SystemTray *m_sysTray;
    PantherWatcher *m_watcher;
};
#endif // MAINWINDOW_H

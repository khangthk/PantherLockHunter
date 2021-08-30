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
    void autoStartHunter();
    void updateStatusBar();
    void setupClearLogTimer();

public slots:
    void onUpdateStatusBar(const bool running, const int totalFileDeleted);
    void onMessageReceived(const QString &mess);

private:
    Ui::MainWindow *ui;
    QLabel *m_statusIcon;
    QLabel *m_statusText;
    QLabel *m_totalFile;
    SystemTray *m_sysTray;
    QTimer m_timer;
};
#endif // MAINWINDOW_H

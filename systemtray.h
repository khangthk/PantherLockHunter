#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QAction>
#include <QMainWindow>
#include <QMenu>
#include <QSystemTrayIcon>

class SystemTray : public QMainWindow
{
    Q_OBJECT
public:
    explicit SystemTray(QWidget *parent = nullptr);

    void showMessageRunningInSystemTray();
    void showMessageAlreadyRunning();
    void updateMenu();

private slots:
    void onOpen();
    void onIconClicked(QSystemTrayIcon::ActivationReason reason);

private:
    QAction *m_openAction;
    QAction *m_quitAction;
    QSystemTrayIcon *m_trayIcon;
    QMenu *m_trayIconMenu;

    void createActions();
    void createMenuIcon();
};

#endif // SYSTEMTRAY_H

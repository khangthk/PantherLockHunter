#include "systemtray.h"

#include <QCoreApplication>

SystemTray::SystemTray(QWidget *parent) : QMainWindow(parent)
{
    createActions();
    createMenuIcon();
    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &SystemTray::onIconClicked);
}

void SystemTray::showMessageRunningInSystemTray()
{
    QString title = "Panther Lock Hunter";
    QString body = "The program will keep running in the system tray.";
    m_trayIcon->showMessage(title, body);
}

void SystemTray::showMessageAlreadyRunning()
{
    QString title = "Panther Lock Hunter";
    QString body = "An instance of the application is already running.";
    m_trayIcon->showMessage(title, body);
}

void SystemTray::updateMenu()
{
    if (parentWidget()->isVisible()) {
        m_openAction->setText("&Hide");
    } else {
        m_openAction->setText("&Show");
    }
}

void SystemTray::onOpen()
{
    if (m_openAction->text() == "&Show") {
        parentWidget()->showNormal();
        parentWidget()->activateWindow();
    } else if (m_openAction->text() == "&Hide") {
        parentWidget()->hide();
    }

    updateMenu();
}

void SystemTray::onIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        parentWidget()->showNormal();
        parentWidget()->activateWindow();
        m_openAction->setText("&Hide");
        break;
    case QSystemTrayIcon::MiddleClick:
        break;
    default:
        break;
    }
}

void SystemTray::createActions()
{
    m_openAction = new QAction("&Show", this);
    connect(m_openAction, &QAction::triggered, this, &SystemTray::onOpen);

    m_quitAction = new QAction("&Quit", this);
    connect(m_quitAction, &QAction::triggered, qApp, &QCoreApplication::exit);
}

void SystemTray::createMenuIcon()
{
    m_trayIconMenu = new QMenu(this);
    m_trayIconMenu->addAction(m_openAction);
    m_trayIconMenu->addAction(m_quitAction);

    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setContextMenu(m_trayIconMenu);
    m_trayIcon->setIcon(QIcon(":/icons/app.ico"));
    m_trayIcon->setToolTip("Panther Lock Hunter");
    m_trayIcon->show();
}

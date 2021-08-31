#include "mainwindow.h"
#include "setting.h"
#include "ui_mainwindow.h"

#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_timer(nullptr)
{
    ui->setupUi(this);
    setCentralWidget(ui->tabParent);

    m_sysTray = new SystemTray(this);
    m_watcher = new PantherWatcher(this);

    m_statusIcon = new QLabel(ui->statusbar);
    m_statusText = new QLabel(ui->statusbar);
    m_totalFile = new QLabel(ui->statusbar);
    ui->statusbar->addPermanentWidget(m_statusIcon, 0);
    ui->statusbar->addPermanentWidget(m_statusText, 1);
    ui->statusbar->addPermanentWidget(m_totalFile, 2);

    connect(this, &MainWindow::statusOfWatcherChanged, ui->tabMain, &TabMain::onStatusOfWatcherChanged);

    connect(m_watcher, &PantherWatcher::addLog, ui->tabLog, &TabLog::onAddLog);
    connect(m_watcher, &PantherWatcher::totalFileDeleteChanged, this, &MainWindow::onUpdateStatusBar);

    connect(ui->tabMain, &TabMain::addWatch, this, &MainWindow::onAddWatch);
    connect(ui->tabMain, &TabMain::deleteWatch, this, &MainWindow::onDeleteWatch);
    connect(ui->tabMain, &TabMain::startWatcher, this, &MainWindow::onStartWatcher);
    connect(ui->tabMain, &TabMain::numberItemOfListChanged, this, &MainWindow::onUpdateStatusBar);

    connect(ui->tabSetting, &TabSetting::moveLockFileToTrash, m_watcher, &PantherWatcher::onMoveLockFileToTrash);
    connect(ui->tabSetting, &TabSetting::initTimerClearLog, ui->tabLog, &TabLog::onInitTimerClearLog);
    connect(ui->tabSetting, &TabSetting::numberItemOfListChanged, this, &MainWindow::onUpdateStatusBar);
    connect(ui->tabSetting, &TabSetting::numberItemOfListChanged, m_watcher, &PantherWatcher::onInitLockFileList);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::starHunter(const bool autoStart)
{
    if (autoStart) {
        onStartWatcher();
    }
    emit statusOfWatcherChanged(m_watcher->isRunning());
    ui->tabLog->onInitTimerClearLog();
}

void MainWindow::onUpdateStatusBar()
{
    if (m_watcher->isRunning()) {
        QPixmap pixmap;
        if (Setting::getWatchFolders().empty() || Setting::getLockFiles().empty()) {
            pixmap = QPixmap(":/icons/yellow.svg").scaledToHeight(ui->statusbar->height() / 2);
        } else {
            pixmap = QPixmap(":/icons/green.svg").scaledToHeight(ui->statusbar->height() / 2);
        }
        m_statusIcon->setPixmap(pixmap);
        m_statusText->setText("Hunting");

        if (!m_timer.isActive()) {
            QObject::connect(&m_timer, &QTimer::timeout, [&](){
                static int i = 0;
                QString text = "Hunting";
                switch (i++) {
                case 0:
                    break;
                    text += " --";
                case 1:
                    text += " -\\-";
                    break;
                case 2:
                    text += " -|-";
                    break;
                case 3:
                    text += " -/-";
                    break;
                default:
                    text += " —";
                    i = 0;
                    break;
                }
                m_statusText->setText(text);
            });

            m_timer.start(100);
        }
    } else {
        m_timer.stop();

        QPixmap pixmap(QPixmap(":/icons/red.svg").scaledToHeight(ui->statusbar->height() / 2));
        m_statusIcon->setPixmap(pixmap);
        m_statusText->setText("Stop");
    }

    m_totalFile->setText("Deleted Files: " + QString::number(m_watcher->getTotalFileDeleted()));
}

void MainWindow::onSingleApplicationMessageReceived(const QString &message)
{
    qDebug() << message;
    if (m_sysTray != nullptr) {
        m_sysTray->showMessageAlreadyRunning();
        m_sysTray->updateMenu();
    }
}

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);

    onUpdateStatusBar();
    if (m_sysTray != nullptr) {
        m_sysTray->updateMenu();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    hide();
    event->ignore();
    if (m_sysTray != nullptr) {
        m_sysTray->showMessageRunningInSystemTray();
        m_sysTray->updateMenu();
    }
}

void MainWindow::onAddWatch(const QString &dir)
{
    if (m_watcher->isRunning()) {
        m_watcher->addPath(dir);
    }
}

void MainWindow::onDeleteWatch(const QStringList &listDir)
{
    foreach (auto &dir, listDir) {
        if (m_watcher->isRunning()) {
            m_watcher->removePath(dir);
        }
    }
}

void MainWindow::onStartWatcher()
{
    if (m_watcher->isRunning()) {
        m_watcher->stop();
        qDebug() << "Stop Hunter";
    } else {
        m_watcher->start();
        qDebug() << "Start Hunter";

        // Add current watch dir list
        QStringList listDir = Setting::getWatchFolders();
        foreach (auto &dir, listDir) {
            if (m_watcher->isRunning()) {
                m_watcher->addPath(dir);
            }
        }
    }

    emit statusOfWatcherChanged(m_watcher->isRunning());
}

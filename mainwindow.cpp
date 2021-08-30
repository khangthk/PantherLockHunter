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

    m_statusIcon = new QLabel(ui->statusbar);
    m_statusText = new QLabel(ui->statusbar);
    m_totalFile = new QLabel(ui->statusbar);
    ui->statusbar->addPermanentWidget(m_statusIcon, 0);
    ui->statusbar->addPermanentWidget(m_statusText, 1);
    ui->statusbar->addPermanentWidget(m_totalFile, 2);

    connect(ui->tabMain, &TabMain::updateStatusBar, this, &MainWindow::onUpdateStatusBar);
    connect(ui->tabMain, &TabMain::appendLog, ui->tabLog, &TabLog::onAppendLog);
    connect(ui->tabSetting, &TabSetting::updateLockFiles, ui->tabMain, &TabMain::onUpdateLockFiles);
    connect(ui->tabSetting, &TabSetting::updateStatusBar, ui->tabMain, &TabMain::onUpdateStatusBar);
    connect(ui->tabSetting, &TabSetting::setupClearLogTimer, ui->tabLog, &TabLog::onClearLogTimer);
    connect(this, &MainWindow::autoStartHunter, ui->tabMain, &TabMain::onAutoStartHunter);
    connect(this, &MainWindow::updateStatusBar, ui->tabMain, &TabMain::onUpdateStatusBar);
    connect(this, &MainWindow::setupClearLogTimer, ui->tabLog, &TabLog::onClearLogTimer);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::starHunter(const bool autoStart)
{
    if (autoStart) {
        emit autoStartHunter();
    }

    emit setupClearLogTimer();
}

void MainWindow::onUpdateStatusBar(const bool running, const int totalFileDeleted)
{
    if (running) {
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

    m_totalFile->setText("Deleted Files: " + QString::number(totalFileDeleted));
}

void MainWindow::onMessageReceived(const QString &mess)
{
    qDebug() << mess;
    if (m_sysTray != nullptr) {
        m_sysTray->showMessageAlreadyRunning();
        m_sysTray->updateMenu();
    }
}

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);

    emit updateStatusBar();
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

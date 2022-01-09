#include "mainwindow.h"
#include "setting.h"
#include "ui_mainwindow.h"

#include <QCloseEvent>
#include <QMessageBox>
#include <QPainter>

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
    m_empty = new QLabel(ui->statusbar);

    m_scanProgressBar = new QProgressBar(ui->statusbar);
    m_scanProgressBar->setRange(0, 0);
    m_scanProgressBar->setFixedHeight(10);
    m_scanProgressBar->setFixedWidth(130);
    m_scanProgressBar->setTextVisible(false);

    ui->statusbar->addPermanentWidget(m_statusIcon, 0);
    ui->statusbar->addPermanentWidget(m_statusText, 1);
    ui->statusbar->addPermanentWidget(m_totalFile, 1);
    ui->statusbar->addPermanentWidget(m_empty, 1);
    ui->statusbar->addPermanentWidget(m_scanProgressBar, 1);

    connect(this, &MainWindow::statusOfWatcherChanged, ui->tabMain, &TabMain::onUpdateButtonStart);
    connect(this, &MainWindow::statusOfWatcherChanged, this, &MainWindow::onUpdateHunterStatus);

    connect(m_watcher, &PantherWatcher::addLog, ui->tabLog, &TabLog::onAddLog);
    connect(m_watcher, &PantherWatcher::totalFileDeleteChanged, this, &MainWindow::onUpdateTotalDeletedStatus);

    connect(ui->tabMain, &TabMain::addWatch, this, &MainWindow::onAddWatch);
    connect(ui->tabMain, &TabMain::deleteWatch, this, &MainWindow::onDeleteWatch);
    connect(ui->tabMain, &TabMain::startWatcher, this, &MainWindow::onStartWatcher);
    connect(ui->tabMain, &TabMain::numberItemOfListChanged, this, &MainWindow::onUpdateHunterStatus);
    connect(ui->tabMain, &TabMain::startScan, this, &MainWindow::onScanStarted);
    connect(ui->tabMain, &TabMain::scanDone, this, &MainWindow::onScanDone);

    connect(ui->tabSetting, &TabSetting::moveLockFileToTrash, m_watcher, &PantherWatcher::onMoveLockFileToTrash);
    connect(ui->tabSetting, &TabSetting::initTimerClearLog, ui->tabLog, &TabLog::onInitTimerClearLog);
    connect(ui->tabSetting, &TabSetting::numberItemOfListChanged, this, &MainWindow::onUpdateHunterStatus);
    connect(ui->tabSetting, &TabSetting::numberItemOfListChanged, m_watcher, &PantherWatcher::onInitLockFileList);
    connect(ui->tabSetting, &TabSetting::numberItemOfListChanged, ui->tabMain, &TabMain::onUpdateButtonScan);
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

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);

    onUpdateHunterStatus();
    onUpdateTotalDeletedStatus();
    onUpdateScanProgressBar();

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

void MainWindow::onSingleApplicationMessageReceived(const QString &message)
{
    qDebug() << message;
    if (m_sysTray != nullptr) {
        m_sysTray->showMessageAlreadyRunning();
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

void MainWindow::onUpdateHunterStatus()
{
    QColor iconColor = QColor(107, 190, 102); //green

    if (m_watcher->isRunning()) {
        if (Setting::getWatchFolders().empty() || Setting::getLockFiles().empty()) {
            iconColor = QColor(255, 160, 0); //yellow
        }

        m_statusText->setText("Hunting");

        auto fnSetAnimationText = [&]() {
            static int i = 0;
            QString text = "Hunting";
            switch (i++) {
            case 0:
                text += " -\\-";
                break;
            case 1:
                text += " -|-";
                break;
            case 2:
                text += " -/-";
                break;
            default:
                text += " —";
                i = 0;
                break;
            }
            m_statusText->setText(text);
        };

        if (!m_timer.isActive()) {
            connect(&m_timer, &QTimer::timeout, fnSetAnimationText);
            m_timer.start(150);
        }
    } else {
        m_timer.stop();
        disconnect(&m_timer, &QTimer::timeout, 0, 0);
        m_statusText->setText("Stop");
        iconColor = QColor(251, 0, 0); //red
    }

    QPixmap iconPixmap(ui->statusbar->height() / 2, ui->statusbar->height() / 2);
    iconPixmap.fill(Qt::transparent);
    QPainter painter(&iconPixmap);
    painter.setBrush(iconColor);
    painter.setPen(Qt::transparent);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawEllipse(iconPixmap.rect());
    m_statusIcon->setPixmap(iconPixmap);
}

void MainWindow::onUpdateTotalDeletedStatus()
{
    m_totalFile->setText("Deleted Files: " + QString::number(m_watcher->getTotalFileDeleted()));
}

void MainWindow::onUpdateScanProgressBar()
{
    if (ui->tabMain->scanning()) {
        m_scanProgressBar->show();
        m_empty->hide();
    } else {
        m_scanProgressBar->hide();
        m_empty->show();
    }
}

void MainWindow::onScanStarted()
{
    onUpdateScanProgressBar();
}

void MainWindow::onScanDone(const int numberFilesDeleted, const QStringList &details)
{
    if (!isVisible()) {
        showNormal();
    }

    onUpdateScanProgressBar();

    if (details.empty()) {
        QMessageBox::information(this, "Scan Lock File", "Lock file not found.");
    } else {
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setWindowTitle("Scan Lock File");
        if (numberFilesDeleted == details.count() / 2) {
            msgBox.setText(QString("Found and deleted %1 lock file%2.                                                             ")
                                   .arg(numberFilesDeleted).arg(numberFilesDeleted > 1 ? "s" : ""));
        } else {
            msgBox.setText(QString("Deleted %1/%2 lock file%3.                                                                    ")
                                   .arg(numberFilesDeleted).arg(details.count() / 2).arg(numberFilesDeleted > 1 ? "s" : ""));
        }

        QString detailText;
        foreach (auto &detail, details) {
            detailText += detail + "\n";
        }
        msgBox.setDetailedText(detailText);
        msgBox.exec();
    }
}

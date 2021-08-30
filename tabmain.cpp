#include "setting.h"
#include "tabmain.h"
#include "ui_tabmain.h"

#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QFileDialog>

TabMain::TabMain(QWidget *parent) :
    QWidget(parent), ui(new Ui::TabMain)
{
    ui->setupUi(this);

    m_watcher = new PantherWatcher(this);
    ui->listWidget->addItems(Setting::getWatchFolders());
    ui->buttonScan->hide(); //TODO

    connect(ui->buttonAdd, &QAbstractButton::clicked, this, &TabMain::onAdd);
    connect(ui->buttonDelete, &QAbstractButton::clicked, this, &TabMain::onDelete);
    connect(ui->buttonStart, &QAbstractButton::clicked, this, &TabMain::onStart);
    connect(ui->buttonScan, &QAbstractButton::clicked, this, &TabMain::onScan);
    connect(ui->listWidget, &QListWidget::itemSelectionChanged, this, &TabMain::onItemSelectionChanged);
    connect(m_watcher, &PantherWatcher::appendLog, this, &TabMain::onAppendLog);
    connect(m_watcher, &PantherWatcher::totalFileDeleteChanged, this, &TabMain::onTotalFileDeleteChanged);
    connect(this, &TabMain::updateLockFiles, m_watcher, &PantherWatcher::onUpdateLockFiles);
    connect(this, &TabMain::moveToTrash, m_watcher, &PantherWatcher::onMoveToTrash);
}

TabMain::~TabMain()
{
    delete ui;
}

PantherWatcher *TabMain::getWatcher() const
{
    return m_watcher;
}

void TabMain::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    updateButtonState();
}

void TabMain::onTotalFileDeleteChanged()
{
    onUpdateStatusBar();
}

void TabMain::onUpdateLockFiles()
{
    if (m_watcher->isRunning()) {
        emit updateLockFiles();
    }
}

void TabMain::onAutoStartHunter()
{
    onStart();
}

void TabMain::onUpdateStatusBar()
{
    emit updateStatusBar(m_watcher->isRunning(), m_watcher->getTotalFileDeleted());
}

void TabMain::onMoveToTrash()
{
    emit moveToTrash();
}

void TabMain::onAdd()
{
    QString lastPath = Setting::getLastPath();
    QString dir = QFileDialog::getExistingDirectory(this, "Select Panther Site", lastPath,
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!dir.isEmpty()) {
        Setting::setLastPath(dir);
        dir = QDir::toNativeSeparators(dir);
        if (ui->listWidget->findItems(dir, Qt::MatchFixedString).empty()) {
            ui->listWidget->addItem(dir);
            if (m_watcher->isRunning()) {
                m_watcher->addPath(dir);
            }
            // Update the watch folder list
            QStringList folders;
            for (int i = 0; i < ui->listWidget->count(); i++) {
                folders << ui->listWidget->item(i)->text();
            }
            Setting::setWatchFolders(folders);
        }
    }

    onUpdateStatusBar();
    updateButtonState();
}

void TabMain::onDelete()
{
    auto items = ui->listWidget->selectedItems();
    foreach (auto item, items) {
        if (m_watcher->isRunning()) {
            m_watcher->removePath(item->text() + QDir::separator());
        }
        delete ui->listWidget->takeItem(ui->listWidget->row(item));
    }
    // Update the watch folder list
    QStringList folders;
    for (int i = 0; i < ui->listWidget->count(); i++) {
        folders << ui->listWidget->item(i)->text();
    }
    Setting::setWatchFolders(folders);

    onUpdateStatusBar();
    updateButtonState();
}

void TabMain::onStart()
{
    if (m_watcher->isRunning()) {
        m_watcher->stop();
        ui->buttonStart->setToolTip("Stop Hunter");
        qDebug() << "Stop Hunter";
    } else {
        m_watcher->start();
        ui->buttonStart->setToolTip("Start Hunter");
        qDebug() << "Start Hunter";
        // Add watch folder
        QStringList list;
        getPantherSiteList(list);
        foreach (auto &dir, list) {
            if (m_watcher->isRunning()) {
                m_watcher->addPath(dir);
            }
        }
    }

    onUpdateStatusBar();
    updateButtonState();
}

void TabMain::onScan()
{
    //TODO
}

void TabMain::onItemSelectionChanged()
{
    updateButtonState();
}

void TabMain::onAppendLog(const QString &log)
{
    emit appendLog(log);
}

void TabMain::updateButtonState()
{
    ui->buttonDelete->setEnabled(!ui->listWidget->selectedItems().empty());
    ui->buttonScan->setEnabled(ui->listWidget->count() > 0);

    if (m_watcher->isRunning()) {
        ui->buttonStart->setText("Stop");
    } else {
        ui->buttonStart->setText("Start");
    }
}

void TabMain::getPantherSiteList(QStringList &list)
{
    list.clear();
    for(int i = 0; i < ui->listWidget->count(); ++i) {
        list.append(ui->listWidget->item(i)->text());
    }
}

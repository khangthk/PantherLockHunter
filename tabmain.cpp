#include "setting.h"
#include "tabmain.h"
#include "ui_tabmain.h"

#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QFileDialog>

TabMain::TabMain(QWidget *parent) :
    QWidget(parent), ui(new Ui::TabMain), m_isScanning(false), m_scanLockFile(nullptr)
{
    ui->setupUi(this);
    ui->listWidget->addItems(Setting::getWatchFolders());

    connect(ui->buttonAdd, &QAbstractButton::clicked, this, &TabMain::onAdd);
    connect(ui->buttonDelete, &QAbstractButton::clicked, this, &TabMain::onDelete);
    connect(ui->buttonStart, &QAbstractButton::clicked, this, &TabMain::onStart);
    connect(ui->buttonScan, &QAbstractButton::clicked, this, &TabMain::onScan);
    connect(ui->listWidget, &QListWidget::itemSelectionChanged, this, &TabMain::onListIndexChanged);
}

TabMain::~TabMain()
{
    delete ui;
}

bool TabMain::scanning() const
{
    return m_isScanning;
}

void TabMain::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    onListIndexChanged();
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
            // Update the watch folder list
            QStringList folders;
            for (int i = 0; i < ui->listWidget->count(); i++) {
                folders << ui->listWidget->item(i)->text();
            }
            Setting::setWatchFolders(folders);

            emit addWatch(dir);
            emit numberItemOfListChanged();
        }
    }

    onListIndexChanged();
}

void TabMain::onDelete()
{
    auto items = ui->listWidget->selectedItems();
    QStringList listDir;
    foreach (auto item, items) {
        listDir.append(item->text() + QDir::separator());
        delete ui->listWidget->takeItem(ui->listWidget->row(item));
    }

    if (!listDir.empty()) {
        // Update the watch folder list
        QStringList folders;
        for (int i = 0; i < ui->listWidget->count(); i++) {
            folders << ui->listWidget->item(i)->text();
        }
        Setting::setWatchFolders(folders);

        emit deleteWatch(listDir);
        emit numberItemOfListChanged();
    }

    onListIndexChanged();
}

void TabMain::onStart()
{
    emit startWatcher();
}

void TabMain::onScan()
{
    if (!m_isScanning) {
        m_isScanning = true;
        m_scanLockFile = new ScanLockFile(this);

        connect(m_scanLockFile, &ScanLockFile::scanDone, this, &TabMain::onScanLockFileDone);
        connect(m_scanLockFile, &ScanLockFile::finished, m_scanLockFile, &QObject::deleteLater);

        m_scanLockFile->start();
        emit startScan();
    } else {
        m_isScanning = false;
        m_scanLockFile->cancel();
        m_scanLockFile->wait();
    }

    updateButtonScan();
}

void TabMain::onListIndexChanged()
{
    updateButtonDelete();
    updateButtonScan();
}

void TabMain::onScanLockFileDone(const int numberFilesDeleted, const QStringList &details)
{
    m_isScanning = false;
    updateButtonScan();
    emit scanDone(numberFilesDeleted, details);
}

void TabMain::onUpdateButtonStart(const bool isRunning)
{
    if (isRunning) {
        ui->buttonStart->setText("Stop");
        ui->buttonStart->setToolTip("Stop Hunter");
    } else {
        ui->buttonStart->setText("Start");
        ui->buttonStart->setToolTip("Start Hunter");
    }
}

void TabMain::onUpdateButtonScan()
{
    updateButtonScan();
}

void TabMain::updateButtonDelete()
{
    ui->buttonDelete->setEnabled(!ui->listWidget->selectedItems().empty());
}

void TabMain::updateButtonScan()
{
    ui->buttonScan->setEnabled(!Setting::getWatchFolders().empty() && !Setting::getLockFiles().empty());
    if (m_isScanning) {
        ui->buttonScan->setText("Cancel");
        ui->buttonScan->setToolTip("Cancel scanning");
    } else {
        ui->buttonScan->setText("Scan");
        ui->buttonScan->setToolTip("Scan and delete the lock files in Panther sites");
    }
}

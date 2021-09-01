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

    ui->listWidget->addItems(Setting::getWatchFolders());
    ui->buttonScan->hide(); //TODO

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
    //TODO
}

void TabMain::onListIndexChanged()
{
    updateButtonDelete();
    updateButtonScan();
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

void TabMain::updateButtonDelete()
{
    ui->buttonDelete->setEnabled(!ui->listWidget->selectedItems().empty());
}

void TabMain::updateButtonScan()
{
    ui->buttonScan->setEnabled(ui->listWidget->count() > 0);
}

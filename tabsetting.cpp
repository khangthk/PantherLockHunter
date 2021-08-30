#include "setting.h"
#include "tabsetting.h"
#include "ui_tabsetting.h"

#include <QFileDialog>

TabSetting::TabSetting(QWidget *parent) :
    QWidget(parent), ui(new Ui::TabSetting)
{
    ui->setupUi(this);

    initData();

    connect(ui->checkBoxRunOnSystemStartup, &QCheckBox::stateChanged, this, &TabSetting::onRunOnSystemStartup);
    connect(ui->checkBoxHideWindowAtLaunch, &QCheckBox::stateChanged, this, &TabSetting::onHideMainWindowWhenOpen);
    connect(ui->checkBoxAutoStartHunter, &QCheckBox::stateChanged, this, &TabSetting::onAutoStartHunter);
    connect(ui->checkBoxMoveToTrash, &QCheckBox::stateChanged, this, &TabSetting::onMoveToTrash);
    connect(ui->checkBoxAutoClearLog, &QCheckBox::stateChanged, this, &TabSetting::onAutoClearLog);
    connect(ui->comboBoxHours, &QComboBox::currentIndexChanged, this, &TabSetting::onHoursIndexChanged);
    connect(ui->buttonAdd, &QAbstractButton::clicked, this, &TabSetting::onAdd);
    connect(ui->buttonDelete, &QAbstractButton::clicked, this, &TabSetting::onDelete);
    connect(ui->buttonDefault, &QAbstractButton::clicked, this, &TabSetting::onDefault);
    connect(ui->listWidget, &QListWidget::itemSelectionChanged, this, &TabSetting::onItemSelectionChanged);
}

TabSetting::~TabSetting()
{
    delete ui;
}

void TabSetting::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    updateButtonState();
}

void TabSetting::onRunOnSystemStartup(int state)
{
    Setting::setRunOnSystemStartup(state != 0);

    //Check again and update state
    bool isCheck = Setting::getRunOnSystemStartup();
    ui->checkBoxRunOnSystemStartup->setCheckState(isCheck ? Qt::Checked : Qt::Unchecked);
}

void TabSetting::onHideMainWindowWhenOpen(int state)
{
    Setting::setHideWindowAtLaunch(state != 0);
}

void TabSetting::onAutoStartHunter(int state)
{
    Setting::setAutoStartHunter(state != 0);
}

void TabSetting::onMoveToTrash(int state)
{
    Setting::setMoveToTrash(state != 0);
    emit moveToTrash()
        ;}

void TabSetting::onAutoClearLog(int state)
{
    Setting::setAutoClearLog(state != 0);
    emit setupClearLogTimer();
}

void TabSetting::onHoursIndexChanged(int index)
{
    Setting::setClearLogHours(index + 1);
    if (Setting::getAutoClearLog()) {
        emit setupClearLogTimer();
    }
}

void TabSetting::onAdd()
{
    QString lastPath = Setting::getLastPath();
    QString filePath = QFileDialog::getOpenFileName(this, "Select Lock File", lastPath);

    if (!filePath.isEmpty()) {
        auto fInfo = QFileInfo(filePath);
        QString fileName = fInfo.fileName();
        lastPath = QDir::toNativeSeparators(fInfo.absolutePath());
        Setting::setLastPath(lastPath);

        if (ui->listWidget->findItems(fileName, Qt::MatchFixedString).empty()) {
            qDebug() << "Add lock file: " + fileName;
            ui->listWidget->addItem(fileName);

            QStringList files;
            for (int i = 0; i < ui->listWidget->count(); i++) {
                files << ui->listWidget->item(i)->text();
            }
            Setting::setLockFiles(files);
        }
    }

    emit updateLockFiles();
    emit updateStatusBar();
    updateButtonState();
}

void TabSetting::onDelete()
{
    auto items = ui->listWidget->selectedItems();
    foreach (auto item, items) {
        qDebug() << "Delete lock file: " + item->text();
        delete ui->listWidget->takeItem(ui->listWidget->row(item));
    }

    QStringList files;
    for (int i = 0; i < ui->listWidget->count(); i++) {
        files << ui->listWidget->item(i)->text();
    }
    Setting::setLockFiles(files);

    emit updateLockFiles();
    emit updateStatusBar();
    updateButtonState();
}

void TabSetting::onDefault()
{
    ui->listWidget->clear();
    ui->listWidget->addItems(Setting::getDefaultLockFiles());
    qDebug() << "Default lock file";
    Setting::setLockFiles(Setting::getDefaultLockFiles());

    emit updateLockFiles();
    emit updateStatusBar();
    updateButtonState();
}

void TabSetting::onItemSelectionChanged()
{
    updateButtonState();
}

void TabSetting::initData()
{
    bool isCheck = Setting::getRunOnSystemStartup();
    ui->checkBoxRunOnSystemStartup->setCheckState(isCheck ? Qt::Checked : Qt::Unchecked);

    isCheck = Setting::getHideWindowAtLaunch();
    ui->checkBoxHideWindowAtLaunch->setCheckState(isCheck ? Qt::Checked : Qt::Unchecked);

    isCheck = Setting::getAutoStartHunter();
    ui->checkBoxAutoStartHunter->setCheckState(isCheck ? Qt::Checked : Qt::Unchecked);

    isCheck = Setting::getMoveToTrash();
    ui->checkBoxMoveToTrash->setCheckState(isCheck ? Qt::Checked : Qt::Unchecked);

    isCheck = Setting::getAutoClearLog();
    ui->checkBoxAutoClearLog->setCheckState(isCheck ? Qt::Checked : Qt::Unchecked);

    int hours = Setting::getClearLogHours();
    ui->comboBoxHours->setCurrentIndex(hours - 1);

    ui->listWidget->addItems(Setting::getLockFiles());
}

void TabSetting::updateButtonState()
{
    ui->buttonDelete->setEnabled(!ui->listWidget->selectedItems().empty());
}



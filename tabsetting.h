#ifndef TABSETTING_H
#define TABSETTING_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class TabSetting; }
QT_END_NAMESPACE

class TabSetting : public QWidget
{
    Q_OBJECT

public:
    explicit TabSetting(QWidget *parent = nullptr);
    ~TabSetting();

protected:
    void showEvent(QShowEvent *event) override;

signals:
    void moveLockFileToTrash();
    void initTimerClearLog();
    void numberItemOfListChanged(); // send this signal to update status bar green or yellow

private slots:
    void onRunOnSystemStartup(int state);
    void onHideMainWindowWhenOpen(int state);
    void onAutoStartHunter(int state);
    void onMoveToTrash(int state);
    void onAutoClearLog(int state);
    void onHoursIndexChanged(int index);
    void onAdd();
    void onDelete();
    void onDefault();
    void onListIndexChanged();

private:
    Ui::TabSetting *ui;

    void initData();
    void updateButtonDeleteState();
};

#endif // TABSETTING_H

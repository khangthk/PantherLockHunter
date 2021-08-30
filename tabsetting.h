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
    void updateLockFiles();
    void updateStatusBar();
    void moveToTrash();
    void setupClearLogTimer();

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
    void onItemSelectionChanged();

private:
    Ui::TabSetting *ui;

    void initData();
    void updateButtonState();
};

#endif // TABSETTING_H

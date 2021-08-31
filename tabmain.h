#ifndef TABMAIN_H
#define TABMAIN_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class TabMain; }
QT_END_NAMESPACE

class TabMain : public QWidget
{
    Q_OBJECT

public:
    explicit TabMain(QWidget *parent = nullptr);
    ~TabMain();

protected:
    void showEvent(QShowEvent *event) override;

signals:
    void addWatch(const QString &dir);
    void deleteWatch(const QStringList &listDir);
    void startWatcher();
    void numberItemOfListChanged(); // send this signal to update status bar green or yellow

public slots:
    void onStatusOfWatcherChanged(const bool isRunning);

private slots:
    void onAdd();
    void onDelete();
    void onStart();
    void onScan();
    void onListIndexChanged();

private:
    Ui::TabMain *ui;

    void updateButtonDeleteState();
    void updateButtonScanState();
};

#endif // TABMAIN_H

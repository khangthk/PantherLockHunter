#ifndef TABLOG_H
#define TABLOG_H

#include <QTimer>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class TabLog; }
QT_END_NAMESPACE

class TabLog : public QWidget
{
    Q_OBJECT

public:
    explicit TabLog(QWidget *parent = nullptr);
    ~TabLog();

public slots:
    void onAddLog(const QString &log);
    void onInitTimerClearLog();

private slots:
    void onClear();

private:
    Ui::TabLog *ui;
    QTimer m_timerClearLog;
};

#endif // TABLOG_H

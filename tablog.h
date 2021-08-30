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
    void onClear();
    void onAppendLog(const QString &log);
    void onClearLogTimer();

private:
    Ui::TabLog *ui;
    QTimer m_clearLogTimer;
};

#endif // TABLOG_H

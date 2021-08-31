#include "setting.h"
#include "tablog.h"
#include "ui_tablog.h"

TabLog::TabLog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabLog)
{
    ui->setupUi(this);

    connect(ui->buttonClear, &QAbstractButton::clicked, this, &TabLog::onClear);
}

TabLog::~TabLog()
{
    delete ui;
}

void TabLog::onClear()
{
    ui->plainTextEdit->clear();
}

void TabLog::onAddLog(const QString &log)
{
    ui->plainTextEdit->moveCursor(QTextCursor::End);
    ui->plainTextEdit->insertPlainText(log);
    ui->plainTextEdit->appendPlainText("");
}

void TabLog::onInitTimerClearLog()
{
    m_timerClearLog.stop();

    if (Setting::getAutoClearLog()) {
        QObject::connect(&m_timerClearLog, &QTimer::timeout, [&](){
            onClear();
        });
        m_timerClearLog.start(1000 * 60 * 60 * Setting::getClearLogHours());
    }
}

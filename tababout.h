#ifndef TABABOUT_H
#define TABABOUT_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class TabAbout; }
QT_END_NAMESPACE

class TabAbout : public QWidget
{
    Q_OBJECT

public:
    explicit TabAbout(QWidget *parent = nullptr);
    ~TabAbout();

private:
    Ui::TabAbout *ui;
};

#endif // TABABOUT_H

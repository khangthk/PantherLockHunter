#include "tababout.h"
#include "ui_tababout.h"

#include <QApplication>

TabAbout::TabAbout(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabAbout)
{
    ui->setupUi(this);
    ui->version->setText("Version 1.0.1.0");

    connect(ui->buttonAboutQt, &QAbstractButton::clicked, qApp, &QApplication::aboutQt);
}

TabAbout::~TabAbout()
{
    delete ui;
}


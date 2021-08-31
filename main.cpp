#include "mainwindow.h"
#include "setting.h"

#include <QApplication>
#include <qtsingleapplication.h>

int main(int argc, char *argv[])
{
    SharedTools::QtSingleApplication a(QApplication::applicationName(), argc, argv);
    if (a.sendMessage("Wake up!")) {
        return 0;
    }

    // Use for setting
    a.setOrganizationName("Med-Aid");
    a.setApplicationName("PantherLockHunter");

    MainWindow w;
    Setting::getHideWindowAtLaunch() ? w.hide() : w.show();
    w.starHunter(Setting::getAutoStartHunter());

    a.setActivationWindow(&w);
    QObject::connect(&a, &SharedTools::QtSingleApplication::messageReceived, &w, &MainWindow::onSingleApplicationMessageReceived);

    return a.exec();
}

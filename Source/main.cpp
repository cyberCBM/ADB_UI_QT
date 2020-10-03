#include "mainwindow.hpp"

#include <QApplication>
#include <settingmanager.hpp>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("MyOrganization");
    a.setOrganizationDomain("MyDomain");
    a.setApplicationName("MyAppName");
    SettingManager settings;
    MainWindow w;

    w.show();
    return a.exec();
}

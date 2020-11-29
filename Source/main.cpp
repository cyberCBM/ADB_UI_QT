#include "mainwindow.hpp"

#include <QApplication>
#include <QIcon>
#include <settingmanager.hpp>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("CyberCBM");
    a.setOrganizationDomain("CBM");
    a.setApplicationName("ADB_UI_QT");

    SettingManager settings;

    MainWindow w;
    w.setWindowTitle("ADB_UI_QT");
    w.show();

    return a.exec();
}

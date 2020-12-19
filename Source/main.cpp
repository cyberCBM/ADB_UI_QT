#include "mainwindow.hpp"

#include <QApplication>

#include <settingmanager.hpp>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("CyberCBM");
    app.setOrganizationDomain("CBM");
    app.setApplicationName("ADB_UI_QT");

    SettingManager settings;

    MainWindow window;
    window.setWindowTitle("ADB_UI_QT");
    window.show();

    return app.exec();
}

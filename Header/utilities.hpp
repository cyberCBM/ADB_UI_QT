#pragma once

#include <QString>
#include <QMessageBox>
#include <QProcess>
#include <settingmanager.hpp>

#define ADB_PATH "ADB_PATH"
#define ADB_DEVICE "ADB_DEVICE"


inline void checkADBPath(const QString &adbPath, QWidget* widget) {
    if(adbPath.isEmpty()) {
        QMessageBox *msgBox = new QMessageBox(widget);
        msgBox->setText("ADB Setting");
        msgBox->setWindowModality(Qt::NonModal);
        msgBox->setInformativeText("ADB path is missing, set ADB path!");
        msgBox->setStandardButtons(QMessageBox::Ok);
        msgBox->exec();
    }
}

inline QByteArray runProcess(const QString &argument) {
    QProcess myProcess;
    myProcess.setProgram(argument);
    myProcess.start();
    myProcess.waitForFinished();
    // read adb devices data to array
    return myProcess.readAllStandardOutput();
}

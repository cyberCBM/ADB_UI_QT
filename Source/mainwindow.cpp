#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "shellterm.hpp"
#include "fileexplorer.hpp"
#include "packagemanager.hpp"
#include "aboutdialog.hpp"
#include "utilities.hpp"
#include "apkdragdropwidget.hpp"

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_UI(new Ui::MainWindow)
{
    m_UI->setupUi(this);

    auto adbStr = SettingManager::value(ADB_PATH).toString();
    m_UI->adbPathTE->setText(adbStr);
    getConnectedDevices();

    m_customDropWidget = new apkDragDropWidget(m_UI->installGroupBox);
    m_UI->installHLayout->addWidget(m_customDropWidget);


    //setAcceptDrops(true);
}

MainWindow::~MainWindow()
{
    delete m_UI;
    delete m_customDropWidget;
}

void MainWindow::getConnectedDevices()
{
    m_UI->deviceComboBox->clear();
    // create process object

    QString adbPathStr = SettingManager::value(ADB_PATH).toString();
    checkADBPath(adbPathStr, this);

    QString adbCMD = adbPathStr + "/adb devices";
    qDebug() << "program is: " << adbCMD;
    QByteArray output = runProcess(adbCMD);

    //auto output = runProcess(adbCMD);
    qDebug() << "output is: " << output;

    // now parse devices
    int lastItem = 0;

    while(lastItem != -1) {
        auto nind = output.indexOf("\n", lastItem);
        auto tind = output.indexOf("\t", nind);
        if(nind!= -1 && tind!= -1)
        {
            auto device = output.mid(nind+1, (tind-nind)-1);
            if(lastItem==0)
                SettingManager::setValue(ADB_DEVICE, device);
            m_UI->deviceComboBox->addItem(device);
        }
        lastItem = tind;
    }

}

void MainWindow::on_explorePushButton_clicked()
{
    FileExplorer *explorer = new FileExplorer(0);
    explorer->show();
}

void MainWindow::on_adbShellPushButton_clicked()
{
    ShellTerm *shell = new ShellTerm(0);
    shell->show();
}

void MainWindow::on_uninstallPushButton_clicked()
{
    PackageManager *pm = new PackageManager(0);
    pm->show();
}

void MainWindow::on_refreshPushButton_clicked()
{
    getConnectedDevices();
}


void MainWindow::on_browsePushButton_clicked()
{
    QString filePath = QFileDialog::getExistingDirectory(this, tr("Open Directory"),"/home",
                                                         QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
    m_UI->adbPathTE->setText(filePath);

}

void MainWindow::on_adbPathTE_textChanged(const QString &arg1)
{
    SettingManager::setValue(ADB_PATH, arg1);
}

void MainWindow::on_deviceComboBox_currentIndexChanged(const QString &arg1)
{
    SettingManager::setValue(ADB_DEVICE, arg1);
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog *about= new AboutDialog(new QDialog(this));
    about->show();
}

void MainWindow::on_actionExplore_triggered()
{
    on_explorePushButton_clicked();
}

void MainWindow::on_actionInstall_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Any"), "~/", tr("APK Files (*.apk)"));
    qDebug() << "Selected file is: " << fileName;

    QString currentDevice = SettingManager::value(ADB_DEVICE).toString();
    if(currentDevice.isEmpty())
        return;
    QString program = QString("adb -s %1 install %2").arg(currentDevice, fileName);
    qDebug() << "program is: " << program;

    // read adb devices data to array
    auto output = runProcess(program);
    qDebug() << "output is: " << output; // if empty then need to show error
    // TODO: implment UI logger

    //QByteArray badOutput = myProcess.readAllStandardError();
    //qDebug() << "bad output is: " << badOutput;
}

void MainWindow::on_actionShell_triggered()
{
    on_adbShellPushButton_clicked();
}

void MainWindow::on_actionUninstall_triggered()
{
    on_uninstallPushButton_clicked();
}

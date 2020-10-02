#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "shellterm.hpp"
#include "fileexplorer.hpp"
#include "packagemanager.hpp"
#include "utilities.hpp"

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_UI(new Ui::MainWindow)
{
    m_UI->setupUi(this);
    getConnectedDevices();
}

MainWindow::~MainWindow()
{

}

void MainWindow::getConnectedDevices()
{
    m_UI->deviceComboBox->clear();

    // create process object


    auto adbStr = m_UI->adbPathTE->text();
    checkADBPath(adbStr, this);

    QString adbCMD = m_UI->adbPathTE->text() + "/adb devices";

    auto output = runProcess(adbCMD);
    qDebug() << "output is: " << output;

    // now parse devices
    int lastItem = 0;

    while(lastItem != -1) {
        auto nind = output.indexOf("\n", lastItem);
        auto tind = output.indexOf("\t", nind);
        if(nind!= -1 && tind!= -1)
        {
            //outputData << output.mid(nind, tind);
            m_UI->deviceComboBox->addItem(output.mid(nind+1, (tind-nind)-1));
        }
        lastItem = tind;
    }

}

// show right message
void MainWindow::showStandard()
{
    //QByteArray strdata = myProcess.readAllStandardOutput();
    //qDebug() << "standard: " << strdata;
}

// show wrong message
void MainWindow::showError()
{
    //QByteArray strdata = myProcess.readAllStandardError();
    //qDebug() << "error: " << strdata;
}

void MainWindow::on_explorePushButton_clicked()
{
    FileExplorer *explorer = new FileExplorer(0, m_UI->deviceComboBox->currentText());
    explorer->show();
}

void MainWindow::on_installPushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Any"), "~/", tr("APK Files (*.apk)"));
    qDebug() << "Selected file is: " << fileName;

    QString program = QString("adb -s %1 install \"%2\"").arg(m_UI->deviceComboBox->currentText(), fileName);
    qDebug() << "program is: " << program;

    // read adb devices data to array
    auto output = runProcess(program);

    qDebug() << "output is: " << output;

    //QByteArray badOutput = myProcess.readAllStandardError();
    //qDebug() << "bad output is: " << badOutput;
}

void MainWindow::on_adbShellPushButton_clicked()
{
    ShellTerm *shell = new ShellTerm(0, m_UI->deviceComboBox->currentText());
    shell->show();
}

void MainWindow::on_uninstallPushButton_clicked()
{
    PackageManager *pm = new PackageManager(0, m_UI->deviceComboBox->currentText());
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

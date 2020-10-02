#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "shellterm.hpp"
#include "fileexplorer.hpp"
#include "packagemanager.hpp"

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //    ui->deviceComboBox->addItem("devices");
    //    ui->deviceComboBox->addItem("devices2");
    //    ui->deviceComboBox->addItem("devices556");
    getConnectedDevices();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getConnectedDevices()
{
    ui->deviceComboBox->clear();

    // create process object
    QProcess myProcess;
    myProcess.start("adb devices");
    myProcess.waitForFinished();

    // read adb devices data to array
    QByteArray output = myProcess.readAllStandardOutput();
    qDebug() << "output is: " << output;

    // now parse devices
    int lastItem = 0;

    while(lastItem != -1) {
        auto nind = output.indexOf("\n", lastItem);
        auto tind = output.indexOf("\t", nind);
        if(nind!= -1 && tind!= -1)
        {
            //outputData << output.mid(nind, tind);
            ui->deviceComboBox->addItem(output.mid(nind+1, (tind-nind)-1));
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
    FileExplorer *explorer = new FileExplorer(0, ui->deviceComboBox->currentText());
    explorer->show();
}

void MainWindow::on_installPushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Any"), "~/", tr("APK Files (*.apk)"));
    qDebug() << "Selected file is: " << fileName;

    QString program = QString("adb -s %1 install \"%2\"").arg(ui->deviceComboBox->currentText(), fileName);
    program.remove(QRegExp("[\\n\\t\\r]"));
    program.remove(QChar('\\', Qt::CaseInsensitive));
    qDebug() << "program is: " << program;

    // create process object
    QProcess myProcess;
    myProcess.start(program);
    myProcess.waitForFinished();

    // read adb devices data to array
    QByteArray output = myProcess.readAllStandardOutput();
    QByteArray badOutput = myProcess.readAllStandardError();
    qDebug() << "output is: " << output;
    qDebug() << "bad output is: " << badOutput;
}

void MainWindow::on_adbShellPushButton_clicked()
{
    ShellTerm *shell = new ShellTerm(0, ui->deviceComboBox->currentText());
    shell->show();
}

void MainWindow::on_uninstallPushButton_clicked()
{
    PackageManager *pm = new PackageManager(0, ui->deviceComboBox->currentText());
    pm->show();
}

void MainWindow::on_refreshPushButton_clicked()
{
    std::cout << "just check";
    getConnectedDevices();
}


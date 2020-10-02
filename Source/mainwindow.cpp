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

    getConnectedDevices();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resetComboBox()
{
    for(int i = 0; i < ui->deviceComboBox->count(); i++)
    {
        ui->deviceComboBox->removeItem(i);
    }
}

QString MainWindow::substring(QString string, int start, int end)
{
    return string.mid(start, end-start);
}

void MainWindow::getConnectedDevices()
{
    // reset devices list combobox
    resetComboBox();

    // reset devices array
    devices.clear();

    // create process object
    QProcess myProcess;
    myProcess.start("adb devices");
    myProcess.waitForFinished();

    // read adb devices data to array
    QStringList outputData;
    QByteArray output = myProcess.readAllStandardOutput();
    qDebug() << "output is: " << output;

    // now parse devices
    int lastItem = 0;
    for(int i = 0; i < output.size(); i++)
    {
        if(output[i] == '\n' || output[i] == '\t')
        {
            outputData << substring(QString(output), lastItem, i);
            lastItem = i;
        }
    }

    // add to class device list array
    for(int i = 1; i < outputData.size()-1; i++)
    {
        if(i % 2 == 1)
        {
            devices << outputData[i];
        }
    }

    // add to devices list
    ui->deviceComboBox->addItems(devices);

    for (int i = 0 ; i < ui->deviceComboBox->count() ; ++i) {
        ui->deviceComboBox->setItemData(i, Qt::AlignCenter, Qt::TextAlignmentRole);
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



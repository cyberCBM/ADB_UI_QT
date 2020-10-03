#include "shellterm.hpp"
#include "ui_ShellTerm.h"

ShellTerm::ShellTerm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShellTerm)
{
    ui->setupUi(this);

    shell = std::shared_ptr<QProcess>();
    QString currentDevice = SettingManager::value(ADB_DEVICE).toString();
    qDebug() << "current device is: " << currentDevice;

    if(!currentDevice.isEmpty())
    {
        // start process
        QString program = QString("adb -s %1 ShellTerm").arg(currentDevice);
        outputData = runProcess(program);
        outputData += "~ \n";
        ui->terminal->setText(QString(outputData));
    }

    connect(shell.get(), SIGNAL(readyReadStandardOutput()), this, SLOT(outputReady()));
    connect(shell.get(), SIGNAL(readyReadErrorOutput()), this, SLOT(outputReady()));
}

ShellTerm::~ShellTerm()
{
    delete ui;
}

void ShellTerm::outputReady()
{
    outputData += shell->readAllStandardOutput();

    // append outputData
    outputData += "~ " + ui->command->text() + "\n";

    ui->terminal->setText(QString(outputData));
    ui->terminal->verticalScrollBar()->setSliderPosition(ui->terminal->verticalScrollBar()->maximum());

    qDebug() << outputData;
}

void ShellTerm::on_send_clicked()
{
    QByteArray dataToSend = ui->command->text().toLocal8Bit();
    dataToSend.append("\n");

    // append outputData
    outputData += "~ " + ui->command->text() + "\n";

    ui->terminal->setText(QString(outputData));
    ui->terminal->verticalScrollBar()->setSliderPosition(ui->terminal->verticalScrollBar()->maximum());

    qDebug() << "data is: " << dataToSend;
    shell->write(dataToSend);
}

void ShellTerm::closeEvent(QCloseEvent *event)
{
    outputData.clear();
}

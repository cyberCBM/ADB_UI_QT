#include "shellterm.hpp"
#include "ui_ShellTerm.h"

ShellTerm::ShellTerm(QWidget *parent) :
    QWidget(parent),
    m_shellProcess(nullptr),
    m_UI(new Ui::ShellTerm)
{
    m_UI->setupUi(this);

    m_shellProcess = std::make_shared<QProcess>();
    QString currentDevice = SettingManager::value(ADB_DEVICE).toString();
    //qDebug() << "current device is: " << currentDevice;

    if(!currentDevice.isEmpty())
    {
        QString adbPathStr = SettingManager::value(ADB_PATH).toString();
        checkADBPath(adbPathStr, this);

        // start process
        QString program = QString("%1/adb -s %2 shell").arg(adbPathStr, currentDevice);
        m_shellProcess->start(program);

        m_outputData += "~ \n";
        m_UI->terminal->setText(QString(m_outputData));
    }

    connect(m_shellProcess.get(), SIGNAL(readyReadStandardOutput()), this, SLOT(outputReady()));
    connect(m_shellProcess.get(), SIGNAL(readyReadErrorOutput()), this, SLOT(outputReady()));
}

ShellTerm::~ShellTerm()
{
    delete m_UI;
}

void ShellTerm::outputReady()
{
    m_outputData += m_shellProcess->readAllStandardOutput();

    // append outputData
    m_outputData += "~ " + m_UI->command->text() + "\n";

    m_UI->terminal->setText(QString(m_outputData));
    m_UI->terminal->verticalScrollBar()->setSliderPosition(m_UI->terminal->verticalScrollBar()->maximum());

    //qDebug() << m_outputData;
}

void ShellTerm::on_send_clicked()
{
    QByteArray dataToSend = m_UI->command->text().toLocal8Bit();
    dataToSend.append("\n");

    // append outputData
    m_outputData += "~ " + m_UI->command->text() + "\n";

    m_UI->terminal->setText(QString(m_outputData));
    m_UI->terminal->verticalScrollBar()->setSliderPosition(m_UI->terminal->verticalScrollBar()->maximum());

    //qDebug() << "data is: " << dataToSend;
    m_shellProcess->write(dataToSend);
}

void ShellTerm::closeEvent(QCloseEvent *event)
{
    m_outputData.clear();
}

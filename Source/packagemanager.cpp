#include "packagemanager.hpp"
#include "ui_PackageManager.h"

PackageManager::PackageManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PackageManager)
{
    ui->setupUi(this);

    model = new QStringListModel(this);

    adb = std::make_shared<QProcess>();

    // now add data to ui
    parseData();
}

PackageManager::~PackageManager()
{
    delete ui;
}

QString PackageManager::substring(QString string, int start, int end)
{
    return string.mid(start, end-start);
}

QString PackageManager::removeGarbage(QString string)
{
    return string.remove("package:", Qt::CaseInsensitive);
}

void PackageManager::parseData()
{
    QString currentDevice = SettingManager::value(ADB_DEVICE).toString();
    if(currentDevice.isEmpty())
        return;

    QString adbPathStr = SettingManager::value(ADB_PATH).toString();
    checkADBPath(adbPathStr, this);

    // start process
    QString program = QString("%1/adb -s %2 shell pm list packages -3").arg(adbPathStr, currentDevice);
    auto outputData = runProcess(program);
    qDebug() << outputData;

    int lastIndex = 0;

    // reset List
    List.clear();

    // parse data into list
    for(int i = 0; i < outputData.size(); i++)
    {
        if(outputData[i] == '\n')
        {
            QString string = substring(QString(outputData), lastIndex, i);
            string = removeGarbage(string);
            lastIndex = i;
            List << string;
        }
    }

    // set model list
    model->setStringList(List);
    ui->listView->setModel(model);

}

void PackageManager::on_uninstall_clicked()
{
    QString currentDevice = SettingManager::value(ADB_DEVICE).toString();
    if(currentDevice.isEmpty())
        return;

    // prompt user if they want to keep data
    QMessageBox msgBox;
    msgBox.setText("Do you want to keep app data?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();

    QString adbPathStr = SettingManager::value(ADB_PATH).toString();
    checkADBPath(adbPathStr, this);

    switch (ret) {
    case QMessageBox::Yes:
        {
            QModelIndex index = ui->listView->currentIndex();
            QString itemText = index.data(Qt::DisplayRole).toString();

            // start process
            QString program = QString("%1/adb -s %2 uninstall -k %3").arg(adbPathStr, currentDevice, itemText);
            runProcess(program);

            parseData();
        }
        break;
    case QMessageBox::No:
        {
            QModelIndex index = ui->listView->currentIndex();
            QString itemText = index.data(Qt::DisplayRole).toString();

            // start process
            QString program = QString("%1/adb -s %2 uninstall %3").arg(adbPathStr, currentDevice, itemText);
            runProcess(program);

            parseData();
        }
        break;
    default:
        break;
    }
}

void PackageManager::on_install_clicked()
{
    QString currentDevice = SettingManager::value(ADB_DEVICE).toString();
    if(currentDevice.isEmpty())
        return;

    QString adbPathStr = SettingManager::value(ADB_PATH).toString();
    checkADBPath(adbPathStr, this);

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Any"), "~/", tr("APK Files (*.apk)"));
    qDebug() << "Selected file is: " << fileName;

    QString program = QString("%1/adb -s %2 install \"%3\"").arg(adbPathStr, currentDevice, fileName);
    program.remove(QRegExp("[\\n\\t\\r]"));
    program.remove(QChar('\\', Qt::CaseInsensitive));
    qDebug() << "program is: " << program;

    // read adb devices data to array
    QByteArray output = runProcess(program);
    qDebug() << "output is: " << output;

//    QByteArray badOutput = myProcess.readAllStandardError();
//    qDebug() << "bad output is: " << badOutput;

    parseData();
}



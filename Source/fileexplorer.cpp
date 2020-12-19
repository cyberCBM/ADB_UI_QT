#include "fileexplorer.hpp"
#include "ui_FileExplorer.h"

FileExplorer::FileExplorer(QWidget *parent) :
    QWidget(parent),
    m_UI(new Ui::FileExplorer),
    m_shellProcess(nullptr),
    m_ADBProcess(nullptr)
{
    m_UI->setupUi(this);

    m_slModel = new QStringListModel(this);

    // set current dir
    m_UI->currentDir->setText("/");
    m_shellProcess = std::make_shared<QProcess>();
    m_ADBProcess = std::make_shared<QProcess>();

    connect(m_ADBProcess.get(), SIGNAL(readyReadStandardOutput()), this, SLOT(adbOutputReady()));
    connect(m_UI->files,SIGNAL(doubleClicked(const QModelIndex)),this,SLOT(ItemClicked(QModelIndex)));

    QString currentDevice = SettingManager::value(ADB_DEVICE).toString();
    //qDebug() << "current device is: " << currentDevice;

    QString adbPathStr = SettingManager::value(ADB_PATH).toString();
    checkADBPath(adbPathStr, this);

    if(!currentDevice.isEmpty())
    {
        // start process
        QString program = QString("%1/adb -s %2 shell").arg(adbPathStr, currentDevice);
        //qDebug() << "program is: " << program;
        m_shellProcess->start(program);
        // send ls command
        m_shellProcess->write("clear\n");
        m_shellProcess->write("ls -F\n");
        m_shellProcess->waitForFinished(1000);
        outputReady();
    }
}

FileExplorer::~FileExplorer(){
    delete m_UI;
}

QString FileExplorer::substring(const QString &str, int start, int end)
{
    return str.mid(start, end-start);
}

QString FileExplorer::removeUnessaryChars(const QString &str)
{
    if(str.isEmpty())
        return "";
    QString result = str;

    if(result[0] == '\xa')
    {
        result.remove(0, 1);
    }

    if(result[result.size()] == '\x0')
    {
        result.remove(result.size(), 1);
    }

    if(result[result.size()-1] == "/" || result[result.size()-1] == "@")
    {
        result.remove(result.size()-1, 1);
    }

    return result;
}

void FileExplorer::ItemClicked (QModelIndex index)
{
    // clear data
    m_stringList.clear();
    QString newDir = removeUnessaryChars(index.data().toString());
    goToDirectory(newDir);
}

void FileExplorer::on_up_clicked()
{
    m_shellProcess->write("cd ..\n");
    m_shellProcess->write("clear\n");
    m_shellProcess->write("ls -F\n");
    m_shellProcess->waitForFinished(1000);
    outputReady();

    // update current dir
    m_shellProcess->write("pwd\n");
    m_shellProcess->waitForFinished(500);
    getDirectory();
}

void FileExplorer::getDirectory()
{
    QByteArray outputData = m_shellProcess->readAllStandardOutput();
    //qDebug() << "Current dir is: " << outputData;
    auto showText = removeUnessaryChars(QString(outputData));
    m_UI->currentDir->setText(showText);
}

void FileExplorer::on_upload_clicked()
{
    QString currentDevice = SettingManager::value(ADB_DEVICE).toString();
    if(currentDevice.isEmpty())
        return;

    QString adbPathStr = SettingManager::value(ADB_PATH).toString();
    checkADBPath(adbPathStr, this);

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Any"), "~/", tr("Any (*)"));
    //qDebug() << "Selected file is: " << fileName;

    // now create new process to upload file
    QString adbCommand = QString("%1/adb -s %2 push %3 %4").arg(adbPathStr, currentDevice, fileName, substring(m_UI->currentDir->text(), 0, m_UI->currentDir->text().size()-1));
    //qDebug() << "program is: " << adbCommand;
    m_ADBProcess->start(adbCommand);

    // now refresh
    m_shellProcess->write("clear\n");
    m_shellProcess->write("ls -F\n");
    m_shellProcess->waitForFinished(1000);
    outputReady();
}

void FileExplorer::on_download_clicked()
{
    QString currentDevice = SettingManager::value(ADB_DEVICE).toString();
    if(currentDevice.isEmpty())
        return;

    QString adbPathStr = SettingManager::value(ADB_PATH).toString();
    checkADBPath(adbPathStr, this);

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    "~/",
                                                    tr("Any (*)"));
    //qDebug() << "Selected file is: " << fileName;

    // now create new process to upload file
    QModelIndex index = m_UI->files->currentIndex();
    QString itemText = index.data(Qt::DisplayRole).toString();
    QString filePath = substring(m_UI->currentDir->text(), 0, m_UI->currentDir->text().size()-1) + "/" + substring(itemText, 1, itemText.size());
    QString adbCommand = QString("%1/adb -s %2 pull %3 %4").arg(adbPathStr, currentDevice, filePath, fileName);
    //qDebug() << "program is: " << adbCommand;
    m_ADBProcess->start(adbCommand);
}

void FileExplorer::on_deleteBtn_clicked()
{
    // get file name
    QModelIndex index = m_UI->files->currentIndex();
    QString itemText = index.data(Qt::DisplayRole).toString();
    QString command = QString("rm -rf %1").arg(itemText);
    //qDebug() << "Delete command is: " << command;

    m_shellProcess->write("clear\n");
    m_shellProcess->write(command.toUtf8());
    m_shellProcess->write("ls -F\n");
    m_shellProcess->waitForFinished(1000);
    outputReady();
}

void FileExplorer::on_rename_clicked()
{
    // get file name
    QModelIndex index = m_UI->files->currentIndex();
    QString itemText = index.data(Qt::DisplayRole).toString();

    // create dialog box prompting user for new file name
    bool ok;
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                         tr("New filename:"), QLineEdit::Normal,
                                         itemText, &ok);

    if(ok && !text.isEmpty())
    {
        // now change filename
        QString command = QString("mv %1 %2").arg(itemText, text);
        //qDebug() << "Move command is: " << command;
        m_shellProcess->write("clear\n");
        m_shellProcess->write(command.toUtf8());
        m_shellProcess->write("ls -F\n");
        m_shellProcess->waitForFinished(1000);
        outputReady();
    }
    else
    {
        // TODO: Present alert dialog saying need to enter text
    }
}

void FileExplorer::on_go_clicked()
{
    QString path = removeUnessaryChars(m_UI->currentDir->text());
    goToDirectory(path);
}

void FileExplorer::goToDirectory(QString& path)
{
    path.insert(0, "cd ");
    path.append("\n");

    // cd into directory
    m_shellProcess->write(path.toLocal8Bit());
    m_shellProcess->write("clear\n");
    m_shellProcess->write("ls -F\n");
    m_shellProcess->waitForFinished(1000);
    outputReady();

    // update current dir
    m_shellProcess->write("pwd\n");
    m_shellProcess->waitForFinished(500);
    getDirectory();

    // set model list
    m_slModel->setStringList(m_stringList);
    m_UI->files->setModel(m_slModel);
}


void FileExplorer::adbOutputReady()
{
    QByteArray outputData = m_ADBProcess->readAllStandardOutput();
    //qDebug() << "adb output: " << outputData;
}

void FileExplorer::outputReady()
{
    QByteArray outputData = m_shellProcess->readAllStandardOutput();
    int lastIndex = 0;

    // get rid of first garbage characters
    outputData.remove(0, 7);

    // clear data
    m_stringList.clear();

    // parse data into list
    for(int i = 0; i < outputData.size(); i++)
    {
        if(outputData[i] == '\n')
        {
            QString string = substring(QString(outputData), lastIndex, i);
            lastIndex = i;
            m_stringList << string;
        }
    }

    // set model list
    m_slModel->setStringList(m_stringList);
    m_UI->files->setModel(m_slModel);

    //qDebug() << "Good output is: " << outputData;
}


void FileExplorer::on_sdCard_clicked()
{
    m_UI->currentDir->setText("/storage/emulated/0");
    on_go_clicked();
}

void FileExplorer::on_currentDir_returnPressed()
{
    on_go_clicked();
}

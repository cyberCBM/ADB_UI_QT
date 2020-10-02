#include "fileexplorer.hpp"
#include "ui_FileExplorer.h"

FileExplorer::FileExplorer(QWidget *parent, QString device) :
    QWidget(parent),
    m_UI(new Ui::FileExplorer),
    m_ShellADB(nullptr)
{
    m_UI->setupUi(this);

    model = new QStringListModel(this);

    gettingDir = false;

    // set current dir
    m_UI->currentDir->setText("/");

    m_ShellADB = std::make_shared<QProcess>();

    currentDevice = device;
    qDebug() << "current device is: " << currentDevice;

    // start process
    QString program = QString("adb -s %1 shell").arg(currentDevice);
    qDebug() << "program is: " << program;
    m_ShellADB->start(program);

    connect(m_ShellADB.get(), SIGNAL(readyReadStandardOutput()), this, SLOT(adbOutputReady()));
    connect(m_UI->files,SIGNAL(doubleClicked(const QModelIndex)),this,SLOT(ItemClicked(QModelIndex)));

    // send ls command
    m_ShellADB->write("clear\n");
    m_ShellADB->write("ls -F\n");
    m_ShellADB->waitForFinished(1000);
    outputReady();
}

FileExplorer::~FileExplorer(){
    delete m_UI;
}

QString FileExplorer::substring(QString string, int start, int end)
{
    return string.mid(start, end-start);
}

QString FileExplorer::removeUnessaryChars(QString str)
{
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
    List.clear();

    QString rawNewDir = removeUnessaryChars(index.data().toString());
    QString newDir = removeUnessaryChars(index.data().toString());
    newDir.insert(0, "cd \"");
    newDir.append("\"\n");

    // cd into directory
    m_ShellADB->write(newDir.toLocal8Bit());
    m_ShellADB->write("clear\n");
    m_ShellADB->write("ls -F\n");
    m_ShellADB->waitForFinished(1000);
    outputReady();

    // update current dir
    gettingDir = true;
    m_ShellADB->write("pwd\n");
    m_ShellADB->waitForFinished(500);
    outputReady();

    // set model list
    model->setStringList(List);
    m_UI->files->setModel(model);
}

void FileExplorer::adbOutputReady()
{
    QByteArray outputData = m_ShellADB->readAllStandardOutput();
    qDebug() << "adb output: " << outputData;
}

void FileExplorer::outputReady()
{
    if(gettingDir == true)
    {
        QByteArray outputData = m_ShellADB->readAllStandardOutput();
        qDebug() << "Current dir is: " << outputData;
        m_UI->currentDir->setText(QString(outputData));
        gettingDir = false;

        return;
    }

    QByteArray outputData = m_ShellADB->readAllStandardOutput();
    int lastIndex = 0;

    // get rid of first garbage characters
    outputData.remove(0, 7);

    // clear data
    List.clear();

    // parse data into list
    for(int i = 0; i < outputData.size(); i++)
    {
        if(outputData[i] == '\n')
        {
            QString string = substring(QString(outputData), lastIndex, i);
            lastIndex = i;
            List << string;
        }
    }

    // set model list
    model->setStringList(List);
    m_UI->files->setModel(model);

    qDebug() << "Good output is: " << outputData;
}


void FileExplorer::on_up_clicked(bool checked)
{
    m_ShellADB->write("cd ..\n");
    m_ShellADB->write("clear\n");
    m_ShellADB->write("ls -F\n");
    m_ShellADB->waitForFinished(1000);
    outputReady();

    // update current dir
    gettingDir = true;
    m_ShellADB->write("pwd\n");
    m_ShellADB->waitForFinished(500);
    outputReady();
}

void FileExplorer::on_upload_clicked(bool checked)
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Any"), "~/", tr("Any (*)"));
    qDebug() << "Selected file is: " << fileName;

    // now create new process to upload file
    QString adbCommand = QString("adb -s %1 push \"%2\" \"%3\"").arg(currentDevice, fileName, substring(m_UI->currentDir->text(), 0, m_UI->currentDir->text().size()-1));
    qDebug() << "program is: " << adbCommand;
    m_ShellADB->start(adbCommand);

    // now refresh
    m_ShellADB->write("clear\n");
    m_ShellADB->write("ls -F\n");
    m_ShellADB->waitForFinished(1000);
    outputReady();
}

void FileExplorer::on_download_clicked(bool checked)
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    "~/",
                                                    tr("Any (*)"));
    qDebug() << "Selected file is: " << fileName;

    // now create new process to upload file
    QModelIndex index = m_UI->files->currentIndex();
    QString itemText = index.data(Qt::DisplayRole).toString();
    QString filePath = substring(m_UI->currentDir->text(), 0, m_UI->currentDir->text().size()-1) + "/" + substring(itemText, 1, itemText.size());
    QString adbCommand = QString("adb -s %1 pull \"%2\" \"%3\"").arg(currentDevice, filePath, fileName);
    qDebug() << "program is: " << adbCommand;
    m_ShellADB->start(adbCommand);
}

void FileExplorer::on_delete_2_clicked(bool checked)
{
    // get file name
    QModelIndex index = m_UI->files->currentIndex();
    QString itemText = index.data(Qt::DisplayRole).toString();
    QString command = QString("rm -rf \"%1\"").arg(itemText);
    command.remove(QRegExp("[\\n\\t\\r]"));
    command.append("\n");

    qDebug() << "Delete command is: " << command;

    m_ShellADB->write("clear\n");
    m_ShellADB->write(command.toUtf8());
    m_ShellADB->write("ls -F\n");
    m_ShellADB->waitForFinished(1000);
    outputReady();
}

void FileExplorer::on_rename_clicked(bool checked)
{
    // get file name
    QModelIndex index = m_UI->files->currentIndex();
    QString itemText = index.data(Qt::DisplayRole).toString();
    itemText.remove(QRegExp("[\\n\\t\\r]"));

    // create dialog box prompting user for new file name
    bool ok;
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                         tr("New filename:"), QLineEdit::Normal,
                                         itemText, &ok);

    if(ok && !text.isEmpty())
    {
        // now change filename
        QString command = QString("mv \"%1\" \"%2\"").arg(itemText, text);
        command.remove(QRegExp("[\\n\\t\\r]"));
        command.append("\n");

        qDebug() << "Move command is: " << command;

        m_ShellADB->write("clear\n");
        m_ShellADB->write(command.toUtf8());
        m_ShellADB->write("ls -F\n");
        m_ShellADB->waitForFinished(1000);
        outputReady();
    }
    else
    {
        // TODO: Present alert dialog saying need to enter text
    }
}

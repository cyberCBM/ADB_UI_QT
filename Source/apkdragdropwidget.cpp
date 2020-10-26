#include "apkdragdropwidget.hpp"
#include "ui_apkdragdropwidget.h"
#include "utilities.hpp"

#include <QDropEvent>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QMimeData>

#include <QDebug>

apkDragDropWidget::apkDragDropWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::apkDragDropWidget)
{
    ui->setupUi(this);
    setAcceptDrops(true);
}

apkDragDropWidget::~apkDragDropWidget()
{
    delete ui;
}

void apkDragDropWidget::dropEvent(QDropEvent *event)
{
    ui->label->setText("Installing APK");
    setStyleSheet ("background-color: rgba(255, 0, 0, 0.2);");
    event->accept ();

    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty())
        return;

    QString fileName = urls.first().toLocalFile();
    installAPKFile(fileName);
}

void apkDragDropWidget::dragEnterEvent(QDragEnterEvent *event)
{
    setStyleSheet ("background-color: rgba(255, 0, 0, 0.1);");
    event->accept ();
}

void apkDragDropWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    setStyleSheet ("background-color: rgba(255, 0, 0, 0);");
    event->accept ();
}

void apkDragDropWidget::installAPKFile(const QString& fileName)
{
    if (fileName.isEmpty() )
        return;

    QString currentDevice = SettingManager::value(ADB_DEVICE).toString();
    if(currentDevice.isEmpty())
        return;

    QString adbPathStr = SettingManager::value(ADB_PATH).toString();
    checkADBPath(adbPathStr, this);

    QString program = QString("%1/adb -s %2 install \"%3\"").arg(adbPathStr, currentDevice, fileName);
    qDebug() << "program is: " << program;

    // read adb devices data to array
    auto output = runProcess(program);
    qDebug() << "output is: " << output; // if empty then need to show error

    ui->label->setText("Drag APK to Install");
    setStyleSheet ("background-color: rgba(255, 0, 0, 0);");
}

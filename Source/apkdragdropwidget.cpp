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
    m_UI(new Ui::apkDragDropWidget)
{
    m_UI->setupUi(this);
    setAcceptDrops(true);
}

apkDragDropWidget::~apkDragDropWidget()
{
    delete m_UI;
}

void apkDragDropWidget::dropEvent(QDropEvent *event)
{
    m_UI->label->setText("Installing APK");
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
    // this command can manage the upgrade of install but not downgrade
    QString program = QString("%1/adb -s %2 install %3").arg(adbPathStr, currentDevice, fileName);
    //qDebug() << "program is: " << program;

    // read adb devices data to array
    auto output = runProcess(program);
    //qDebug() << "output is: " << output; // if empty then need to show error
    if(!output.contains("Success"))
    {
        // reinstall or downgrade the same package
        QString program = QString("%1/adb -s %2 install -r -d %3").arg(adbPathStr, currentDevice, fileName);
        //qDebug() << "program is: " << program;

        // read adb devices data to array
        auto output = runProcess(program);
        //qDebug() << "output is: " << output; // if empty then need to show error
    }
    m_UI->label->setText("Drag APK to Install");
    setStyleSheet ("background-color: rgba(255, 0, 0, 0);");
}

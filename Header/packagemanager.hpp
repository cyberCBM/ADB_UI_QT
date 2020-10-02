#pragma once

#include <QWidget>
#include <QProcess>
#include <QDebug>
#include <QString>
#include <QStringListModel>
#include <QMessageBox>
#include <QFileDialog>

namespace Ui {
class PackageManager;
}

class PackageManager : public QWidget
{
    Q_OBJECT

public:
    explicit PackageManager(QWidget *parent = 0, QString device = "");
    ~PackageManager();

private slots:
    void on_uninstall_clicked();

    void on_install_clicked();

private:
    Ui::PackageManager *ui;
    QProcess *adb;
    QString currentDevice;
    void parseData();
    QStringList List;
    QString substring(QString string, int start, int end);
    QStringListModel *model;
    QString removeGarbage(QString string);
};


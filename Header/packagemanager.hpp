#pragma once

#include <QWidget>
#include <QProcess>
#include <QDebug>
#include <QString>
#include <QStringListModel>
#include <QMessageBox>
#include <QFileDialog>
#include <memory>
#include "utilities.hpp"


namespace Ui {
class PackageManager;
}

class PackageManager : public QWidget
{
    Q_OBJECT

public:
    explicit PackageManager(QWidget *parent = 0);
    ~PackageManager();

private slots:
    void on_uninstall_clicked();
    void on_install_clicked();

private:
    Ui::PackageManager *ui;
    std::shared_ptr<QProcess> adb;

    QStringList List;
    QStringListModel *model;

    void parseData();
    QString substring(QString string, int start, int end);
    QString removeGarbage(QString string);
};


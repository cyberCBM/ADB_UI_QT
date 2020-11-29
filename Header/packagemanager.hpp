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
    Ui::PackageManager *m_UI;
    std::shared_ptr<QProcess> m_ADBProcess;

    QStringList m_stringList;
    QStringListModel *m_slModel;

    void parseData();
    QString substring(const QString &string, int start, int end);
    QString removeGarbage(QString string);
};


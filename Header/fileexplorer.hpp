#pragma once

#include <QWidget>
#include <QProcess>
#include <QDebug>
#include <QScrollBar>
#include <QString>
#include <QStringListModel>
#include <QFileDialog>
#include <QInputDialog>
#include <memory>
#include "utilities.hpp"

namespace Ui {
class FileExplorer;
}

class FileExplorer : public QWidget
{
    Q_OBJECT

public:
    explicit FileExplorer(QWidget *parent = 0);
    ~FileExplorer();
    QString substring(QString string, int start, int end);
    QString removeUnessaryChars(QString str);

private slots:
    void outputReady();
    void adbOutputReady();
    void ItemClicked (QModelIndex index );

    void on_up_clicked(bool checked);
    void on_upload_clicked(bool checked);
    void on_download_clicked(bool checked);
    void on_delete_2_clicked(bool checked);
    void on_rename_clicked(bool checked);

private:
    Ui::FileExplorer *m_UI;
    std::shared_ptr<QProcess> m_ShellADB;
    QStringListModel *model;
    QStringList List;
    bool gettingDir;
};

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
    QString substring(const QString &str, int start, int end);
    QString removeUnessaryChars(const QString &str);

private slots:
    void outputReady();
    void adbOutputReady();
    void ItemClicked (QModelIndex index );

    void on_up_clicked();
    void on_upload_clicked();
    void on_download_clicked();
    void on_rename_clicked();
    void on_go_clicked();
    void on_deleteBtn_clicked();

private:
    Ui::FileExplorer *m_UI;
    std::shared_ptr<QProcess> m_shellProcess;
    std::shared_ptr<QProcess> m_ADBProcess;
    QStringListModel *m_slModel;
    QStringList m_stringList;

    void getDirectory();
    void goToDirectory(QString& path);
};

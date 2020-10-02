#pragma once

#include <QWidget>
#include <QProcess>
#include <QDebug>
#include <QScrollBar>

namespace Ui {
class ShellTerm;
}

class ShellTerm : public QWidget
{
    Q_OBJECT

public:
    explicit ShellTerm(QWidget *parent = 0, QString device = "");
    ~ShellTerm();

private slots:
    void on_send_clicked();
    void outputReady();

protected:
    void closeEvent(QCloseEvent *event);

private:
    QProcess *shell;
    Ui::ShellTerm *ui;
    QString currentDevice;
    QByteArray outputData;
    int positionOfCommand;
};

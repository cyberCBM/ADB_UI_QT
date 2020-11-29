#pragma once

#include <QWidget>
#include <QProcess>
#include <QDebug>
#include <QScrollBar>

#include <memory>
#include "utilities.hpp"

namespace Ui {
class ShellTerm;
}

class ShellTerm : public QWidget
{
    Q_OBJECT

public:
    explicit ShellTerm(QWidget *parent = 0);
    ~ShellTerm();

private slots:
    void on_send_clicked();
    void outputReady();

protected:
    void closeEvent(QCloseEvent *event);

private:
    std::shared_ptr<QProcess>   m_shellProcess;
    Ui::ShellTerm*              m_UI;
    QByteArray                  m_outputData;
    int                         m_positionOfCommand;
};

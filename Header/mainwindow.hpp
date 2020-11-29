#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QDebug>

#include <vector>
#include <memory>

namespace Ui {
class MainWindow;
}

class apkDragDropWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void getConnectedDevices();
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_refreshPushButton_clicked();
    void on_explorePushButton_clicked();
    void on_adbShellPushButton_clicked();
    void on_uninstallPushButton_clicked();
    void on_browsePushButton_clicked();

    void on_adbPathTE_textChanged(const QString &arg1);
    void on_deviceComboBox_currentIndexChanged(const QString &arg1);
    void on_actionAbout_triggered();
    void on_actionExplore_triggered();
    void on_actionInstall_triggered();
    void on_actionShell_triggered();
    void on_actionUninstall_triggered();

private:
    Ui::MainWindow*     m_UI;
    apkDragDropWidget*  m_customDropWidget;
};

#endif // MAINWINDOW_H

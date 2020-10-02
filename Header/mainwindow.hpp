#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QDebug>
#include <vector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void getConnectedDevices();
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void showStandard();
    void showError();

    void on_refreshPushButton_clicked();
    void on_explorePushButton_clicked();
    void on_installPushButton_clicked();
    void on_adbShellPushButton_clicked();
    void on_uninstallPushButton_clicked();

private:
    Ui::MainWindow *ui;
    QStringList devices;
    QProcess *shell;
};

#endif // MAINWINDOW_H

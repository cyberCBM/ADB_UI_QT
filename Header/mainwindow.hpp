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
    void on_browsePushButton_clicked();


    void on_adbPathTE_textChanged(const QString &arg1);

    void on_deviceComboBox_currentIndexChanged(const QString &arg1);

private:
    Ui::MainWindow *m_UI;
    std::unique_ptr<QProcess> m_Shell;
};

#endif // MAINWINDOW_H

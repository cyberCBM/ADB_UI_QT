#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->deviceList->addItem("abd");
    ui->deviceList->addItem("abcd");
    ui->deviceList->addItem("abdas");
    ui->deviceList->addItem("abddasdas");
}

MainWindow::~MainWindow()
{
    delete ui;
}


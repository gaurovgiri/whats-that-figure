#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QScreen>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    move(screen()->geometry().center() - frameGeometry().center()); // opens up window to the centre of the screen
}

MainWindow::~MainWindow()
{
    delete ui;
}


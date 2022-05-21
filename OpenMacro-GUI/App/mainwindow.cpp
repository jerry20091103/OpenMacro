#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->menuPresets->injectDependencies(
                ui->fileNameLabel,
                ui->actionSavePreset);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Test(){
    std::printf("Test\n");
}

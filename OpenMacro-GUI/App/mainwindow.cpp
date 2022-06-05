#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->menuPresets->injectDependencies(
                ui->fileNameLabel,
                ui->actionSavePreset,
                ui->actionSavePresetAuto,
                ui->inputList);
    ui->inputList->injectDependencies(
                ui->menuPresets,
                ui->actionNewInput,
                ui->actionDeleteSelectedInput,
                ui->commandList);
    ui->commandList->injectDependencies(
                ui->inputList,
                ui->configMacroForm,
                ui->actionNewCommand,
                ui->actionDeleteSelectedCommand);
    ui->configMacroForm->injectDependencies(
                ui->commandList,
                ui->mouseXInput,
                ui->mouseYInput,
                ui->mouseScrollInput,
                ui->inputModeInput,
                ui->mouseButtonInput,
                ui->keyInput,
                ui->modifierInput);
//    for(int i = 0; i < 10; ++i)
//        ui->inputList->addItem("Test " + QString::number(i));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Test(){
    std::printf("Test\n");
}

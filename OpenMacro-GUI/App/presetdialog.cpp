#include <QCoreApplication>
#include <QAbstractEventDispatcher>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "exceptionhandler.h"
#include "presetdialog.h"
#include "ui_presetdialog.h"
#include <QThread>

PresetDialog::PresetDialog(QWidget* parent, PresetMenu *presetMenu)
  : QDialog(parent)
  , ui(new Ui::PresetDialog)
  , serialPort(this)
  , presetMenu(presetMenu)
{
    ui->setupUi(this);
    this->setWindowTitle("Connect to Device");
    this->passwordDialog = new PasswordDialog(this, this);
    connect(ui->passwordButton, &QPushButton::clicked, this, [&](bool checked){
        passwordDialog->show();
    });
    serialPort.setBaudRate(QSerialPort::Baud9600);
    serialPort.setDataBits(QSerialPort::Data8);
    serialPort.setParity(QSerialPort::NoParity);
    serialPort.setStopBits(QSerialPort::OneStop);
    serialPort.setFlowControl(QSerialPort::NoFlowControl);
    // [Select port list callback]
    connect(ui->portList, &QComboBox::currentIndexChanged, this, [&](int index){
        if(ui->portList->count() == 0) {
            return;
        }
        const QString& portName = ui->portList->currentText();
        serialPort.setPortName(portName);
        refreshState();
    });
    // [Upload button callback]
    connect(ui->uploadButton, &QPushButton::clicked, this, [&](bool checked){
        qDebug() << "Upload";
        EXCEPT_ALERT(
            auto openMode = QIODeviceBase::OpenMode();
            openMode.setFlag(QIODeviceBase::OpenModeFlag::ReadWrite);
            // somehow, these are required to successfully communicate with arduino micro.
            serialPort.open(openMode);
            serialPort.setBaudRate(QSerialPort::Baud9600);
            serialPort.setDataBits(QSerialPort::Data8);
            serialPort.setParity(QSerialPort::NoParity);
            serialPort.setStopBits(QSerialPort::OneStop);
            serialPort.setFlowControl(QSerialPort::HardwareControl);
            serialPort.close();

            serialPort.open(openMode);
            this->presetMenu->uploadPreset(serialPort);
            serialPort.close();
            // wait for Arduino to save macro
            QThread::msleep(1000);
            // open port with 1200 "magic baud" to force reset Arduino
            if(serialPort.open(openMode))
            {
                serialPort.setBaudRate(QSerialPort::Baud1200);
                serialPort.setDataBits(QSerialPort::Data8);
                serialPort.setParity(QSerialPort::NoParity);
                serialPort.setStopBits(QSerialPort::OneStop);
                serialPort.setFlowControl(QSerialPort::HardwareControl);
                serialPort.close();
            }
            else
                qDebug() << "Failed to reset Arduino";

        );
    });
    // [Download button callback]
    connect(ui->downloadButton, &QPushButton::clicked, this, [&](bool checked){
        qDebug() << "Download";
        QMessageBox::information(this, "Downloading", "Please hold down the encoder button on the macro device.", QMessageBox::Ok);
        EXCEPT_ALERT(
            auto openMode = QIODeviceBase::OpenMode();
            openMode.setFlag(QIODeviceBase::OpenModeFlag::ReadWrite);
            // somehow, these are required to successfully communicate with arduino micro.
            serialPort.open(openMode);
            serialPort.setBaudRate(QSerialPort::Baud9600);
            serialPort.setDataBits(QSerialPort::Data8);
            serialPort.setParity(QSerialPort::NoParity);
            serialPort.setStopBits(QSerialPort::OneStop);
            serialPort.setFlowControl(QSerialPort::HardwareControl);
            serialPort.close();

            serialPort.open(openMode);
            this->presetMenu->downloadPreset(serialPort);
            serialPort.close();
            this->close();
        );

    });

}

PresetDialog::~PresetDialog()
{
    //    if(serialPort.isOpen()) serialPort.close();
}

QSerialPort &PresetDialog::getSerialPort()
{
    return serialPort;
}

void PresetDialog::showEvent(QShowEvent *event)
{
    qDebug() << "Show event";
    timerId = QCoreApplication::instance()->eventDispatcher()->registerTimer(1000, Qt::TimerType::CoarseTimer, this);
    refreshPortList();
    refreshState();
}

void PresetDialog::closeEvent(QCloseEvent *event)
{
    qDebug() << "Close event";
    QCoreApplication::instance()->eventDispatcher()->unregisterTimer(timerId);
    passwordDialog->close();
//    if(serialPort.isOpen()) serialPort.close();
}

void PresetDialog::timerEvent(QTimerEvent *event)
{
    qDebug() << "Timer event";
    refreshPortList();
    refreshState();
}

void PresetDialog::setStatus(const QString &text)
{
    ui->statusLabel->setText("Status: " + text);
}

void PresetDialog::refreshPortList()
{
    auto availablePorts = QSerialPortInfo::availablePorts();
    // Check if port list has changed. If not, do not refresh list.
    bool isDifferent = false;
    foreach(const QSerialPortInfo& port, availablePorts){
        if (port.description() != "Arduino Micro")
            continue;
        if(availablePortNames.find(port.portName()) == availablePortNames.end()) {
            isDifferent = true;
        }
    }
    if(!isDifferent) return;
    availablePortNames.clear();
    ui->portList->clear();
    qDebug() << "Number of available ports: " << availablePorts.size();
    foreach(const QSerialPortInfo& port, availablePorts){
        if (port.description() != "Arduino Micro")
            continue;
        ui->portList->addItem(port.portName() + "-" + port.description());
        availablePortNames.insert(port.portName() + "-" + port.description());
    }
}

void PresetDialog::refreshState()
{
    if(ui->portList->count() > 0){
        setStatus("Port ready");
        ui->uploadButton->setEnabled(presetMenu->hasActivePreset());
        ui->downloadButton->setEnabled(true);
        ui->portList->setEnabled(true);
        ui->passwordButton->setEnabled(true);
    }
    else {
        setStatus("No port available");
        ui->uploadButton->setEnabled(false);
        ui->downloadButton->setEnabled(false);
        ui->portList->setEnabled(false);
        ui->passwordButton->setEnabled(false);
    }
}

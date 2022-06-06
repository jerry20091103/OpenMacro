#include <QCoreApplication>
#include <QAbstractEventDispatcher>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "exceptionhandler.h"
#include "presetdialog.h"
#include "ui_presetdialog.h"

PresetDialog::PresetDialog(QWidget* parent, PresetMenu *presetMenu)
  : QDialog(parent)
  , ui(new Ui::PresetDialog)
  , serialPort(this)
  , presetMenu(presetMenu)
{
    ui->setupUi(this);
    this->setWindowTitle("Connect to Device");
    serialPort.setBaudRate(QSerialPort::Baud9600);
    serialPort.setDataBits(QSerialPort::Data8);
    serialPort.setParity(QSerialPort::NoParity);
    serialPort.setStopBits(QSerialPort::OneStop);
    serialPort.setFlowControl(QSerialPort::NoFlowControl);
    // [Serial port bytes written callback]
    connect(&serialPort, &QSerialPort::bytesWritten, this, [&](int bytesWritten){
        qDebug() << "Bytes written:" << bytesWritten;
        serialPort.close();
    });
    // [Serial port ready read callback]
    connect(&serialPort, &QSerialPort::readyRead, this, [&](){
        qDebug() << "Ready read";
        serialPort.close();
    });
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
            if(timerId >= 0)
                QCoreApplication::instance()->eventDispatcher()->unregisterTimer(timerId);
            serialPort.open(openMode);
            this->presetMenu->uploadPreset(serialPort);
            timerId = QCoreApplication::instance()->eventDispatcher()->registerTimer(1000, Qt::TimerType::CoarseTimer, this);

        );
    });
    // [Download button callback]
    connect(ui->downloadButton, &QPushButton::clicked, this, [&](bool checked){
        qDebug() << "Download";
        EXCEPT_ALERT(
            auto openMode = QIODeviceBase::OpenMode();
            openMode.setFlag(QIODeviceBase::OpenModeFlag::ReadWrite);
            if(timerId >= 0)
                QCoreApplication::instance()->eventDispatcher()->unregisterTimer(timerId);
            serialPort.open(openMode);
            this->presetMenu->downloadPreset(serialPort);
            timerId = QCoreApplication::instance()->eventDispatcher()->registerTimer(1000, Qt::TimerType::CoarseTimer, this);
        );
    });

}

PresetDialog::~PresetDialog()
{
//    if(serialPort.isOpen()) serialPort.close();
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
    if(timerId >= 0)
        QCoreApplication::instance()->eventDispatcher()->unregisterTimer(timerId);
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
        if(availablePortNames.find(port.portName()) == availablePortNames.end()) {
            isDifferent = true;
        }
    }
    if(!isDifferent) return;
    availablePortNames.clear();
    ui->portList->clear();
    qDebug() << "Number of available ports: " << availablePorts.size();
    foreach(const QSerialPortInfo& port, availablePorts){
        ui->portList->addItem(port.portName());
        availablePortNames.insert(port.portName());
    }
}

void PresetDialog::refreshState()
{
    if(ui->portList->count() > 0){
        setStatus("Port ready");
        ui->uploadButton->setEnabled(true);
        ui->downloadButton->setEnabled(true);
        ui->portList->setEnabled(true);
    }
    else {
        setStatus("No port available");
        ui->uploadButton->setEnabled(false);
        ui->downloadButton->setEnabled(false);
        ui->portList->setEnabled(false);
    }
}

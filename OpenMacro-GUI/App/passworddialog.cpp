#include "passworddialog.h"
#include "ui_passworddialog.h"
#include <QThread>
#include <QRegularExpression>

PasswordDialog::PasswordDialog(QWidget *parent, PresetDialog *presetDialog) :
    QDialog(parent),
    ui(new Ui::PasswordDialog),
    passwordValidator(QRegularExpression("[A-Za-z0-9_]+"), this)
{
    ui->setupUi(this);
    this->pwSlots[0] = ui->pwSlot1;
    this->pwSlots[1] = ui->pwSlot2;
    this->pwSlots[2] = ui->pwSlot3;
    this->pwSlots[3] = ui->pwSlot4;
    this->pwSlots[4] = ui->pwSlot5;
    this->pwSlots[5] = ui->pwSlot6;
    this->pwSlots[6] = ui->pwSlot7;
    this->pwSlots[7] = ui->pwSlot8;
    this->pwSlots[8] = ui->pwSlot9;
    for(int i = 0; i < MAX_PASSWORDS; ++i){
        this->pwSlots[i]->setMaxLength(MAX_PASSWORD_LEN);
        this->pwSlots[i]->setValidator(&passwordValidator);
    }
    connect(ui->pwConfirm, &QPushButton::clicked, this, [this, presetDialog](bool checked) {
        Config config;
        config.isPassword = true;
        PasswordConfig& pwConfig = config.passwordConfig;
        for(int i = 0; i < MAX_PASSWORDS; ++i){
            pwConfig.passwords[i].size = this->pwSlots[i]->text().length();
            memcpy(pwConfig.passwords[i].str, this->pwSlots[i]->text().toStdString().c_str(), pwConfig.passwords[i].size * sizeof(char));
        }
        QSerialPort& serialPort = presetDialog->getSerialPort();

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
            if(serialPort.isOpen() && serialPort.isWritable()){
                int bytesWritten = serialPort.write((const char*)&config, sizeof(Config));
                if(bytesWritten == -1) {
                    throw "Failed to write to port " + serialPort.portName() + ", error: " + serialPort.errorString();
                }
                if(!serialPort.waitForBytesWritten(5000)){
                    qDebug() << "Wrote " << bytesWritten << " to" << serialPort.portName();
                    throw "Error: " + serialPort.errorString();
                }
            }
            else throw "Port " + serialPort.portName() + " is not open or writable.";
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
}

PasswordDialog::~PasswordDialog()
{
    delete ui;
}

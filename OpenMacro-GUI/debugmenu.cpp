#include "debugmenu.h"

const char* debugPortName = "COM12";

DebugMenu::DebugMenu(QWidget* parent) : QMenu(parent)
{

}

static QSerialPort* initSerialPort(){
    auto serialPort = new QSerialPort();
    serialPort->setPortName(debugPortName);
    serialPort->setBaudRate(QSerialPort::Baud9600);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);
    return serialPort;
}

void DebugMenu::testSerial(void (*testFunc)(QSerialPort*)){
    QSerialPort* serialPort = initSerialPort();
    testFunc(serialPort);
    delete serialPort;
}

void DebugMenu::testSerialRead(QSerialPort* serialPort){
    qDebug() << "TestSerialRead";
    auto openMode = QIODeviceBase::OpenMode();
    openMode.setFlag(QIODeviceBase::OpenModeFlag::ReadOnly);
    if(serialPort->open(openMode)){
        while (!serialPort->canReadLine())
                serialPort->waitForReadyRead(-1);
        auto readResult = serialPort->readLine ();
        qDebug() << readResult;
        serialPort->close();
    }
    else qDebug() << "Failed to open port" << debugPortName;
}

void DebugMenu::testSerialWrite(QSerialPort* serialPort){
    qDebug() << "TestSerialWrite";
    auto openMode = QIODeviceBase::OpenMode();
    openMode.setFlag(QIODeviceBase::OpenModeFlag::WriteOnly);
    if(serialPort->open(openMode)){
        int bytesWritten = serialPort->write("Hello?\r\n");
        if(bytesWritten == -1) qDebug() << "Failed to write bytes to" << debugPortName;
        else qDebug() << "Wrote " << bytesWritten << " bytes to" << debugPortName;
        serialPort->close();
    }
    else qDebug() << "Failed to open port" << debugPortName;
}

void DebugMenu::onTestSerialWrite(){
    testSerial(testSerialWrite);
}

void DebugMenu::onTestSerialRead(){
    testSerial(testSerialRead);
}

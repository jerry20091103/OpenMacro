#include "debugmenu.h"

const char* debugPortName = "COM5";
QSerialPort::BaudRate baudRate = QSerialPort::Baud9600;

DebugMenu::DebugMenu(QWidget* parent) : QMenu(parent)
{

}

static QSerialPort* initSerialPort(){
    auto serialPort = new QSerialPort();
    serialPort->setPortName(debugPortName);
    serialPort->setBaudRate(baudRate);
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
        auto readResult = serialPort->readAll();
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
        char bytes[8];
        for(int i = 0; i < 8; ++i) bytes[i] = i + 1;
        int bytesWritten = serialPort->write(bytes);
        if(bytesWritten == -1) qDebug() << "Failed to write bytes to" << debugPortName;
        else qDebug() << "Wrote " << bytesWritten << " to" << debugPortName;
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

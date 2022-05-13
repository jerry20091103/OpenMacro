#include "debugmenu.h"

const char* debugPortName = "COM5";

DebugMenu::DebugMenu(QWidget* parent) : QMenu(parent)
{

}

static QSerialPort* initSerialPort(){
    auto serialPort = new QSerialPort();
    serialPort->setPortName(debugPortName);
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
        const auto bytes = new char[8];
        for(int i = 0; i < 8; ++i) bytes[i] = i + 1;
        serialPort->write(bytes);
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

#include "debugmenu.h"

const char* debugPortName = "COM12";

DebugMenu::DebugMenu(QWidget* parent) : QMenu(parent)
{

}

static QSerialPort* initSerialPort(){
    auto serialPort = new QSerialPort();
    auto openMode = QIODeviceBase::OpenMode();
    openMode.setFlag(QIODeviceBase::OpenModeFlag::ReadWrite);

    serialPort->setPortName(debugPortName);

    serialPort->open(openMode);

    serialPort->setBaudRate(baudRate);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);
    return serialPort;
}

void DebugMenu::testSerial(void (*testFunc)(QSerialPort*)){
    QSerialPort* serialPort = initSerialPort();
    testFunc(serialPort);
    serialPort->close();
    delete serialPort;
}

void DebugMenu::testSerialRead(QSerialPort* serialPort){
    qDebug() << "TestSerialRead";
//    auto openMode = QIODeviceBase::OpenMode();
//    openMode.setFlag(QIODeviceBase::OpenModeFlag::ReadOnly);
    if(serialPort->isOpen()){
        while (!serialPort->canReadLine())
                serialPort->waitForReadyRead(-1);
        auto readResult = serialPort->readLine ();
        qDebug() << readResult;
        //serialPort->close();
    }
    else qDebug() << "Failed to open port" << debugPortName;
}

void DebugMenu::testSerialWrite(QSerialPort* serialPort){
    qDebug() << "TestSerialWrite";
<<<<<<< HEAD
//    auto openMode = QIODeviceBase::OpenMode();
//    openMode.setFlag(QIODeviceBase::OpenModeFlag::WriteOnly);
    if(serialPort->isOpen() && serialPort->isWritable()){
        char bytes[8];
        for(int i = 0; i < 8; ++i) bytes[i] = i + 1;
        int bytesWritten = serialPort->write("Hello from Qt");
        serialPort->flush();
        if(bytesWritten == -1) qDebug() << "Failed to write bytes to" << debugPortName;
        else qDebug() << "Wrote " << bytesWritten << " to" << debugPortName;
        //serialPort->close();
=======
    auto openMode = QIODeviceBase::OpenMode();
    openMode.setFlag(QIODeviceBase::OpenModeFlag::WriteOnly);
    if(serialPort->open(openMode)){
        int bytesWritten = serialPort->write("Hello?\r\n");
        if(bytesWritten == -1) qDebug() << "Failed to write bytes to" << debugPortName;
        else qDebug() << "Wrote " << bytesWritten << " bytes to" << debugPortName;
        serialPort->close();
>>>>>>> d4de74249291678d5d152f0eb11a4ab99daf45cd
    }
    else qDebug() << "Failed to open port" << debugPortName;
}

void DebugMenu::onTestSerialWrite(){
    testSerial(testSerialWrite);
}

void DebugMenu::onTestSerialRead(){
    testSerial(testSerialRead);
}

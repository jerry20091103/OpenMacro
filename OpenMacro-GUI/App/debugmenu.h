#ifndef DEBUGMENU_H
#define DEBUGMENU_H

#include <QMenuBar>
#include <QMenu>
#include <QSerialPort>
#include <QDebug>

class DebugMenu : public QMenu
{
    Q_OBJECT
public:
    DebugMenu(QWidget *parent = nullptr);

private:
    static void testSerial(void (*testFunc)(QSerialPort*));
    /// Write a message to serial port.
    static void testSerialWrite(QSerialPort*);
    /// Read everything from the serial port, and then print everything to the console.
    static void testSerialRead(QSerialPort*);
public slots:
    void onTestSerialWrite();
    void onTestSerialRead();
};

#endif // DEBUGMENU_H

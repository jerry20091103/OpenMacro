#ifndef PRESET_H
#define PRESET_H
#include <QFile>
#include <QSerialPort>
#include "commons.h"
/// An in-memory representation of a macro preset file.
/// \brief The Preset class
///
class Preset
{
public:
    struct Input {
        uint16_t delay = 1;
        std::vector<MacroPacket> packets;
    };
    uint8_t expanderAddr[MAX_EXPANDERS];
    MacroPassword passwords[MAX_PASSWORDS];
private:
    std::vector<Input> inputs;
    QMetaObject::Connection readyReadConn, bytesWrittenConn, errorConn;
public:
    Preset();
    /// Load the Preset object with JSON data from the given file.
    void readFrom(QString fileName);
    /// Write the Preset object's JSON data into the given file.
    void saveAs(QString fileName) const;
    /// Load the Preset object with MacroConfig data from the given serial port.
    void loadFromSerial(QSerialPort &serialPort);
    /// Upoad the Preset object with MacroConfig data from the given serial port.
    void uploadToSerial(QSerialPort &serialPort);

    /// The file extension used for macro preset files.
    static const QString fileExtension;
    std::vector<Input> &getInputs();
    const std::vector<Input> &getInputs() const;
    void addInput();
    void popInput();
};

#endif // PRESET_H

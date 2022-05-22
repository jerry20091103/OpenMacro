#ifndef PRESET_H
#define PRESET_H
#include <QFile>
#include "commons.h"
/// An in-memory representation of a macro preset file.
/// \brief The Preset class
///
class Preset
{
    struct Input {
        MacroMode mode = MacroMode::KEYBOARD_MOUSE_CLICK;
        uint8_t delay = 1;
        QString commands = "";
    };
    std::vector<Input> inputs = std::vector<Input>(NUM_BTN_INPUTS);
public:
    Preset();
    /// Load the Preset object with JSON data from the given file.
    void readFrom(QString fileName);
    /// Write the Preset object's JSON data into the given file.
    void saveAs(QString fileName) const;
    /// Load the Preset object with MacroConfig data from the given serial port.
    void loadFromSerial(QString portName);
    /// Upoad the Preset object with MacroConfig data from the given serial port.
    void uploadToSerial(QString portName);

    /// The file extension used for macro preset files.
    static const QString fileExtension;
    const std::vector<Input> &getInputs() const;
    void addInput();
    void popInput();
};

#endif // PRESET_H

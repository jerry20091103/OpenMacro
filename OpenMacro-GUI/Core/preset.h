#ifndef PRESET_H
#define PRESET_H
#include <QFile>
/// An in-memory representation of a macro preset file.
/// \brief The Preset class
///
class Preset
{
public:
    Preset();
    /// Load the Preset object with data from the given file.
    void readFrom(QString fileName);
    /// Write the Preset object's data into the given file.
    void saveAs(QString fileName);

    /// The file extension used for macro preset files.
    static const QString fileExtension;
};

#endif // PRESET_H

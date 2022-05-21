#include "preset.h"
#include <QJsonDocument>
#include <QJsonObject>

const QString Preset::fileExtension = ".omp"; // OMP -> Open Macro Preset

Preset::Preset()
{

}

void Preset::readFrom(QString fileName)
{
    if(!fileName.endsWith(Preset::fileExtension))
        throw "Expected preset file to end with " + Preset::fileExtension;
    // Handle for the file.
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        throw "Failed to open file " + Preset::fileExtension;
    const QByteArray bytes = file.readAll();
    const QJsonObject jsonObject = QJsonDocument::fromJson(bytes).object();
    // TODO: Read from JSON object into class properties.

    file.close();
}

void Preset::saveAs(QString fileName) const
{
    if(!fileName.endsWith(Preset::fileExtension))
        throw "Expected preset file to end with " + Preset::fileExtension;
    // Handle for the file.
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        throw "Failed to open file " + Preset::fileExtension;
    const QJsonObject jsonObject;
    // TOOD: Insert fields into JSON object.

    file.write(QJsonDocument(jsonObject).toJson());
    if(!file.flush())
        throw "Failed to flush file while saving.";

    file.close();
}

void Preset::loadFromSerial(QString portName)
{

}

void Preset::uploadToSerial(QString portName)
{

}

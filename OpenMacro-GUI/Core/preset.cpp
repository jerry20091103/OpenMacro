#include "preset.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCryptographicHash>

const QString Preset::fileExtension = ".omp"; // OMP -> Open Macro Preset
const char* macroModeName[] = { "button", "pointer" };
static MacroMode macroNameToMode(QString name){
    if(name == macroModeName[MacroMode::KEYBOARD_MOUSE_CLICK]) return MacroMode::KEYBOARD_MOUSE_CLICK;
    if(name == macroModeName[MacroMode::MOUSE_MOVE]) return MacroMode::MOUSE_MOVE;
    return MacroMode::KEYBOARD_MOUSE_CLICK;


}
const char* macroModeField = "mode";
const char* macroDelayField = "delay";
const char* macroCommandsField = "commands";
const char* inputArrayField = "macro_actions";

static inline void verifyOMPField(const QJsonObject& jsonObject, const char* fieldName, QJsonValue::Type expectedType){
    if(!jsonObject.contains(fieldName))
        throw "Malformed preset file: Missing field " + QString(fieldName);
    if(jsonObject[fieldName].type() != expectedType)
        throw "Malformed preset file: Field " + QString(fieldName) + " has incorrect type.";
}

const std::vector<Preset::Input> &Preset::getInputs() const
{
    return inputs;
}

void Preset::addInput()
{
    inputs.push_back(Input());
}

void Preset::popInput()
{
    inputs.pop_back();
}

Preset::Preset()
{
    // Default inputs that would make more sense having MOUSE_MOVE as default.
    inputs[BTN_JOY].mode = inputs[ENC_DEC].mode = inputs[ENC_INC].mode = MacroMode::MOUSE_MOVE;
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
    verifyOMPField(jsonObject, inputArrayField, QJsonValue::Array);
    const QJsonArray inputArray = jsonObject[inputArrayField].toArray();
    (*this) = Preset(); // Reset this preset
    int idx = 0;
    foreach(const QJsonValue& val, inputArray){
        // Extend vector to take in JSON object.
        while(idx >= inputs.size()) inputs.push_back(Preset::Input());
        Preset::Input& input = inputs[idx++];
        const QJsonObject& inputObject = val.toObject();
        verifyOMPField(inputObject, macroModeField, QJsonValue::String);
        verifyOMPField(inputObject, macroDelayField, QJsonValue::Double);
        verifyOMPField(inputObject, macroCommandsField, QJsonValue::String);
        input.mode = macroNameToMode(inputObject[macroModeField].toString());
        input.delay = inputObject[macroDelayField].toInt();
        input.commands = inputObject[macroCommandsField].toString();
    }
    file.close();
}

void Preset::saveAs(QString fileName) const
{
    if(!fileName.endsWith(Preset::fileExtension))
        throw "Expected preset file to end with " + Preset::fileExtension;
    // Handle for the file.
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        throw "Failed to open file " + Preset::fileExtension;
    }
    QJsonObject jsonObject;
    // TOOD: Insert fields into JSON object.
    QJsonArray inputArray;
    qDebug() << "Number of inputs " + QString::number(inputs.size());
    foreach(const Preset::Input& input, inputs){
        QJsonObject inputObject;
        const char* modeName = macroModeName[input.mode];
        inputObject[macroModeField] = modeName;
        inputObject[macroDelayField] = input.delay;
        inputObject[macroCommandsField] = input.commands;
        inputArray.append(inputObject);
    }
    jsonObject[inputArrayField] = inputArray;
    file.write(QJsonDocument(jsonObject).toJson());
    if(!file.flush()){
        file.close();
        throw "Failed to flush file while saving.";
    }
    file.close();
}

void Preset::loadFromSerial(QString portName)
{

}

void Preset::uploadToSerial(QString portName)
{

}

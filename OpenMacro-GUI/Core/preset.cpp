#include "preset.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCryptographicHash>

const QString Preset::fileExtension = ".json"; // Open Macro Preset (json)
const char* macroModeField = "mode";
const char* macroDelayField = "delay";
const char* macroCommandsField = "commands";
const char* expanderAddressField = "expander_addresses";
const char* inputArrayField = "macro_actions";
const char* mouseButtonField = "button";
const char* mouseXField = "x";
const char* mouseYField = "y";
const char* mouseWheelField = "wheel";
const char* keyboardButtonField = "button";
const char* modifierField = "modifier";

static inline void verifyOMPField(const QJsonObject& jsonObject, const char* fieldName, QJsonValue::Type expectedType){
    if(!jsonObject.contains(fieldName))
        throw "Malformed preset file: Missing field " + QString(fieldName);
    if(jsonObject[fieldName].type() != expectedType)
        throw "Malformed preset file: Field " + QString(fieldName) + " has incorrect type.";
}

std::vector<Preset::Input> &Preset::getInputs()
{
    return inputs;
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
    for(int i = 0; i < NUM_BTN_INPUTS; ++i)
        addInput();
    for(int i = 0; i < MAX_EXPANDERS; ++i)
        expanderAddr[i] = 0;
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
    verifyOMPField(jsonObject, expanderAddressField, QJsonValue::Array);
    const QJsonArray inputArray = jsonObject[inputArrayField].toArray();
    const QJsonArray addrArray = jsonObject[expanderAddressField].toArray();
    (*this) = Preset(); // Reset this preset
    int idx = 0;
    foreach(const QJsonValue& val, inputArray){
        // Extend vector to take in JSON object.
        while(idx >= inputs.size()) inputs.push_back(Preset::Input());
        Preset::Input& input = inputs[idx++];
        const QJsonObject& inputObject = val.toObject();
        verifyOMPField(inputObject, macroDelayField, QJsonValue::Double);
        verifyOMPField(inputObject, macroCommandsField, QJsonValue::Array);
        input.delay = static_cast<uint16_t>(inputObject[macroDelayField].toInt());
        input.packets.clear();
        const QJsonArray& macroCommandsArray = inputObject[macroCommandsField].toArray();
        int jdx = 0;
        foreach(const QJsonValue& val, macroCommandsArray){
            while(jdx >= input.packets.size()) input.packets.push_back(MacroPacket());
            MacroPacket& packet = input.packets[jdx++];
            const QJsonObject& commandObject = val.toObject();
            verifyOMPField(commandObject, macroModeField, QJsonValue::Double);
            packet.mode = static_cast<MacroMode>(commandObject[macroModeField].toInt(MacroMode::KEYBOARD_MOUSE_CLICK));
            switch(packet.mode) {
            case MacroMode::MOUSE_MOVE:
                verifyOMPField(commandObject, mouseButtonField, QJsonValue::Double);
                verifyOMPField(commandObject, mouseXField, QJsonValue::Double);
                verifyOMPField(commandObject, mouseYField, QJsonValue::Double);
                verifyOMPField(commandObject, mouseWheelField, QJsonValue::Double);
                packet.mouseMove.mouseBtn = static_cast<uint8_t>(commandObject[mouseButtonField].toInt(MOUSE_LEFT));
                packet.mouseMove.mouseX = static_cast<int16_t>(commandObject[mouseXField].toInt(0));
                packet.mouseMove.mouseY = static_cast<int16_t>(commandObject[mouseYField].toInt(0));
                packet.mouseMove.wheel = static_cast<int16_t>(commandObject[mouseWheelField].toInt(0));
                break;
                default:
                case MacroMode::KEYBOARD_MOUSE_CLICK:
                verifyOMPField(commandObject, keyboardButtonField, QJsonValue::Double);
                verifyOMPField(commandObject, modifierField, QJsonValue::Double);
                packet.keycode = static_cast<uint8_t>(commandObject[keyboardButtonField].toInt(0));
                packet.modifierCode = static_cast<uint8_t>(commandObject[modifierField].toInt(0));
                break;
            }
        }

    }
    for(idx = 0; idx < MAX_EXPANDERS; ++idx) {
        expanderAddr[idx] = addrArray[idx].toInt(0);
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
        inputObject[macroDelayField] = (int)input.delay;
        QJsonArray macroCommandsArray = QJsonArray();
        foreach(const MacroPacket& packet, input.packets){
            QJsonObject commandObject;
            commandObject[macroModeField] = packet.mode;
            switch(packet.mode){
                case MacroMode::MOUSE_MOVE:
                commandObject[mouseButtonField] = packet.mouseMove.mouseBtn;
                commandObject[mouseXField] = packet.mouseMove.mouseX;
                commandObject[mouseYField] = packet.mouseMove.mouseY;
                commandObject[mouseWheelField] = packet.mouseMove.wheel;
                default:
                case MacroMode::KEYBOARD_MOUSE_CLICK:
                commandObject[keyboardButtonField] = packet.keycode;
                commandObject[modifierField] = packet.modifierCode;
                break;
            }
            macroCommandsArray.push_back(commandObject);
        }
        inputObject[macroCommandsField] = macroCommandsArray;
        inputArray.append(inputObject);
    }
    jsonObject[inputArrayField] = inputArray;

    QJsonArray addressArray;
    for(int idx = 0; idx < MAX_EXPANDERS; ++idx){
        addressArray.push_back(expanderAddr[idx]);
    }
    jsonObject[expanderAddressField] = addressArray;
    file.write(QJsonDocument(jsonObject).toJson());
    if(!file.flush()){
        file.close();
        throw "Failed to flush file while saving.";
    }
    file.close();
}

void Preset::loadFromSerial(QSerialPort& serialPort)
{
    if(serialPort.isOpen()){
        MacroConfig data;
        serialPort.waitForReadyRead(10000);
        int bytesRead = serialPort.read((char*)&data, sizeof(MacroConfig));
//        if(!serialPort.waitForReadyRead(10000)){
//            qDebug() << "Bytes read:" << bytesRead;
//            throw "Error: " + serialPort.errorString();
//        }
        qDebug() << "Bytes read:" << bytesRead;
        if(bytesRead != sizeof(MacroConfig)) throw "Download failed. Did not read in correct number of bytes.";
        memcpy(expanderAddr, data.expanderAddr, MAX_EXPANDERS * sizeof(uint8_t));
        // Clear any existing inputs to prevent accidents.
        inputs.clear();
        for(int idx = 0; idx < data.numInputs; ++idx){
            Input input;
            MacroAction action = data.inputs[idx];
            input.delay = action.delay;
            // We write MacroPackets one by one here to be on the safe side.
            for(int jdx = 0; jdx < action.size; ++jdx){
                MacroPacket packet;
                std::memcpy(&packet, // write to packet struct
                            data.commandBuffer + action.data + jdx * sizeof(MacroPacket), // start of packet
                            sizeof(MacroPacket)); // size is just the MacroPacket size
                input.packets.push_back(packet);
            }
            inputs.push_back(input);
        }


    }
    else throw "Port " + serialPort.portName() + " is not open.";
}

void Preset::uploadToSerial(QSerialPort& serialPort)
{
    if(serialPort.isOpen() && serialPort.isWritable()){
        Config config;
        config.isPassword = false;
        MacroConfig& data = config.macroConfig;
        // Copy expander addresses
        memcpy(data.expanderAddr, expanderAddr, MAX_EXPANDERS * sizeof(uint8_t));
        data.numInputs = this->inputs.size();
        // We need to keep track of dataPtr to write correct data offsets.
        uint16_t dataPtr = 0;
        int inputIdx = 0;
        foreach(const Input& input, this->inputs){
            MacroAction action;
            action.delay = input.delay;
            action.size = input.packets.size();
            action.data = dataPtr;
            std::memcpy(data.commandBuffer + dataPtr, // Write to command buffer
                        input.packets.data(), // Read from Qt side's data pointer
                        sizeof(MacroPacket) * input.packets.size());
            data.inputs[inputIdx++] = action;
            dataPtr += sizeof(MacroPacket) * input.packets.size();
        }
        int bytesWritten = serialPort.write((const char*)&config, sizeof(Config));

        if(bytesWritten == -1) {
            throw "Failed to write to port " + serialPort.portName() + ", error: " + serialPort.errorString();
        }
        if(!serialPort.waitForBytesWritten(5000)){
            qDebug() << "Wrote " << bytesWritten << " to" << serialPort.portName();
            throw "Error: " + serialPort.errorString();
        }
        qDebug() << "Wrote " << bytesWritten << " to" << serialPort.portName();

    }
    else throw "Port " + serialPort.portName() + " is not open or writable.";

}

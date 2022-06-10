#include "Macros.h"
#include "Hardware.h"

Macros macros;

int16_t Macros::readFromSerial()
{
    int16_t read = 0;
    read += Serial.readBytes((uint8_t *)&config.macroConfig, sizeof(MacroConfig));
    return read;
}

bool Macros::sendToSerial()
{
    Serial.write((uint8_t *)&config, sizeof(config));
}

void Macros::saveToEEPROM()
{
    // first byte is used for valid flag
    EEPROM.write(0, 1);
    EEPROM.put(1, config);
}

bool Macros::readFromEEPROM()
{
    if(EEPROM.read(0) != 1)
    {
        return false;
    }
    EEPROM.get(1, config);
    return true;
}

void Macros::setupMacros()
{
    // add two expanders
    // ! need to test hotswapping expanded pins
    if (config.macroConfig.expanderAddr[0] != 0x00)
    {
    }
}

void Macros::runMacro(uint8_t input)
{
    runningMacro = true;
    if (input >= config.macroConfig.numInputs)
        return;

    uint16_t delay = config.macroConfig.inputs[input].delay;
    for (uint8_t i = 0; i < config.macroConfig.inputs[input].size; i++)
    {
        MacroPacket *packet = (MacroPacket *)&config.macroConfig.commandBuffer[config.macroConfig.inputs[input].data + sizeof(MacroPacket) * i];

        // press modifiers
        Keyboard.press((KeyboardKeycode)packet->modifierCode);

        if (packet->mode == MOUSE_MOVE)
        {
            if (packet->mouseMove.mouseBtn != 0)
            {
                Mouse.press(packet->mouseMove.mouseBtn);
                Mouse.move(packet->mouseMove.mouseX, packet->mouseMove.mouseY, packet->mouseMove.wheel);
                Mouse.release(packet->mouseMove.mouseBtn);
            }
            else
            {
                Mouse.move(packet->mouseMove.mouseX, packet->mouseMove.mouseY, packet->mouseMove.wheel);
            }
        }
        else if (packet->mode == KEYBOARD_MOUSE_CLICK) // KEYBOARD_MOUSE_CLICK
        {
            Keyboard.write((KeyboardKeycode)packet->keycode);
        }

        // release modifier
        Keyboard.release((KeyboardKeycode)packet->modifierCode);
        Keyboard.flush();
        if (delay > 0 && i != config.macroConfig.inputs[input].size-1)
        {
            taskManager.yieldForMicros(1000000 * delay);
        }
    }
    runningMacro = false;
}

void Macros::clearConfig()
{
    memset(&config, 0, sizeof(MacroConfig));
}

// print current config to serial
void Macros::dumpConfig()
{
    Serial.println("Dumping Config");
    Serial.print("expanderAddr: ");
    for (uint8_t i = 0; i < MAX_EXPANDERS; i++)
    {
        Serial.print(config.macroConfig.expanderAddr[i], HEX);
        Serial.print(" ");
    }
    Serial.print("\nnumInputs: ");
    Serial.print(config.macroConfig.numInputs);
    Serial.println("\ninputs:");
    for (uint8_t i = 0; i < config.macroConfig.numInputs; i++)
    {
        Serial.print(String(i) + " size: " + String(config.macroConfig.inputs[i].size) + "\tdelay: " + String(config.macroConfig.inputs[i].delay) + "\tdata: " + String(config.macroConfig.inputs[i].data) + "\n");
        for (uint8_t j = 0; j < config.macroConfig.inputs[i].size; j++)
        {
            MacroPacket *packet = (MacroPacket *)&config.macroConfig.commandBuffer[config.macroConfig.inputs[i].data + sizeof(MacroPacket) * j];
            Serial.print("\tcmd" + String(j) + " mode: " + String(packet->mode) + "\tmod: " + String(packet->modifierCode));
            if (packet->mode == KEYBOARD_MOUSE_CLICK)
            {
                Serial.print("\tkeycode: " + String(packet->keycode) + "\n");
            }
            else if (packet->mode == MOUSE_MOVE)
            {
                Serial.print("\tmouseBtn: " + String(packet->mouseMove.mouseBtn) + "\tmouseX: " + String(packet->mouseMove.mouseX) + "\tmouseY: " + String(packet->mouseMove.mouseY) + "\twheel: " + String(packet->mouseMove.wheel) + "\n");
            }
        }
    }
}
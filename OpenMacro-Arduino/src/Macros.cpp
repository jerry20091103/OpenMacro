#include "Macros.h"
#include "Hardware.h"

Macros macros;

bool Macros::readFromSerial()
{
    Serial.readBytes((uint8_t *)&config, sizeof(config));
}

bool Macros::sendToSerial()
{
    Serial.write((uint8_t *)&config, sizeof(config));
}

void Macros::saveToEEPROM()
{
    uint8_t *p;
    p = (uint8_t *)&config;
    for (int i = 0; i < sizeof(config); i++)
    {
        EEPROM.write(i, *p);
        p++;
    }
}

void Macros::readFromEEPROM()
{
    uint8_t *p;
    p = (uint8_t *)&config;
    for (int i = 0; i < sizeof(config); i++)
    {
        *p = EEPROM.read(i);
        p++;
    }
}

void Macros::setupMacros()
{
    // add two expanders
    // ! need to test hotswapping expanded pins
    if (config.expanderAddr[0] != 0x00)
    {
    }
}

void Macros::runMacro(uint8_t input)
{
    if (input >= config.numInputs)
        return;

    uint16_t delay = config.inputs[input].delay;
    for (uint8_t i = 0; i < config.inputs[input].size; i++)
    {
        MacroPacket *packet = (MacroPacket *)&config.commamdBuffer[config.inputs[input].data];

        // press modifiers
        Keyboard.press(packet->modifierCode);

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
        else // KEYBOARD_MOUSE_CLICK
        {
            Keyboard.write(packet->keycode);
        }

        // release modifier
        Keyboard.release(packet->modifierCode);

        if (delay > 0)
            taskManager.yieldForMicros(1000 * delay);
    }
}

extern MacroConfig config;
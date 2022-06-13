#include "Macros.h"
#include "Hardware.h"
#include "Controls.h"
#include "AESLib.h"

Macros macros;

int16_t Macros::readFromSerial()
{
    return Serial.readBytes((uint8_t *)&config.macroConfig, sizeof(MacroConfig));
}

bool Macros::sendToSerial()
{
    Serial.write((uint8_t *)&config, sizeof(config));
}

void Macros::saveToEEPROM(bool isPassword)
{
    // first byte is used for valid flag
    EEPROM.update(0, 1);
    if (isPassword)
    {
        EEPROM.put(sizeof(MacroConfig) + 2, config.passwordConfig);
    }
    else
    {
        EEPROM.put(1, config.macroConfig);
    }
}

bool Macros::readFromEEPROM(bool isPassword)
{
    if (EEPROM.read(0) != 1)
    {
        return false;
    }
    if (isPassword)
    {
        EEPROM.get(sizeof(MacroConfig) + 2, config.passwordConfig);
    }
    else
    {
        EEPROM.get(1, config.macroConfig);
    }
    return true;
}

uint8_t Macros::setupMacros()
{
    // !debug test passwords
    // uint8_t key[16];
    // for (uint8_t i = 0; i < 16; i++)
    // {
    //     key[i] = rfidUID[i % 4];
    // }
    // aes128_enc_single(key, config.passwordConfig.passwords[0].str);
    // saveToEEPROM(true);
    // readFromEEPROM(false);

    uint8_t expanded = 0;
    // setup expanders
    for (uint8_t i = 0; i < MAX_EXPANDERS; i++)
    {
        if (config.macroConfig.numInputs > NUM_BTN_INPUTS + i * 16)
        {
            // test if address out of range
            if (config.macroConfig.expanderAddr[i] < 0x20 || config.macroConfig.expanderAddr[i] > 0x27)
                continue;
            // test i2c connection
            Wire.beginTransmission(config.macroConfig.expanderAddr[i]);
            if (Wire.endTransmission())
                continue;
            // add expander
            multiIoAddExpander(multiIo, ioFrom23017(config.macroConfig.expanderAddr[i]), 16);
            // setup buttons
            for (uint8_t btn = 0; btn < config.macroConfig.numInputs - NUM_BTN_INPUTS - i * 16 && btn < 16; btn++)
            {
                switches.addSwitch(btn + EXPANDER_PIN_OFFSET + i * 16, BtnPressCallback);
            }
            expanded++;
        }
    }
    return expanded;
}

void Macros::runMacro(uint8_t input)
{
    if (passwordMode)
    {
        if (input > 8)
            return;
        char pswd[17];
        uint8_t key[16];
        readFromEEPROM(true);
        for (uint8_t i = 0; i < 16; i++)
        {
            pswd[i] = config.passwordConfig.passwords[input].str[i];
            key[i] = rfidUID[i % 4];
        }
        pswd[16] = '\0';
        aes128_dec_single(key, pswd);
        Keyboard.print(pswd);
        passwordMode = false;
        displayCurMode();
        readFromEEPROM(false);
    }
    else
    {
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
                // ! test mouse btn 0 !!!
                // if (packet->mouseMove.mouseBtn != 0)
                // {
                Mouse.press(packet->mouseMove.mouseBtn);
                Mouse.move(packet->mouseMove.mouseX, packet->mouseMove.mouseY, packet->mouseMove.wheel);
                Mouse.release(packet->mouseMove.mouseBtn);
                // }
                // else
                // {
                //     Mouse.move(packet->mouseMove.mouseX, packet->mouseMove.mouseY, packet->mouseMove.wheel);
                // }
            }
            else if (packet->mode == KEYBOARD_MOUSE_CLICK) // KEYBOARD_MOUSE_CLICK
            {
                if(packet->keycode == KEY_VOLUME_MUTE)
                    Consumer.write(MEDIA_VOLUME_MUTE);
                else if (packet->keycode == KEY_VOLUME_UP)
                    Consumer.write(MEDIA_VOLUME_UP);
                else if (packet->keycode == KEY_VOLUME_DOWN)
                    Consumer.write(MEDIA_VOLUME_DOWN);
                else
                    Keyboard.write((KeyboardKeycode)packet->keycode);
            }

            // release modifier
            Keyboard.release((KeyboardKeycode)packet->modifierCode);
            if (delay > 0 && i != config.macroConfig.inputs[input].size - 1)
            {
                taskManager.yieldForMicros(1000000 * delay);
            }
        }
    }
}

void Macros::clearConfig()
{
    memset(&config, 0, sizeof(MacroConfig));
}

bool Macros::checkExpanders()
{
    return config.macroConfig.numInputs > NUM_BTN_INPUTS;
}

// print current config to serial
void Macros::dumpConfig()
{
    Serial.println(F("Dumping Config"));
    Serial.print(F("expanderAddr: "));
    for (uint8_t i = 0; i < MAX_EXPANDERS; i++)
    {
        Serial.print(config.macroConfig.expanderAddr[i], HEX);
        Serial.print(" ");
    }
    Serial.print(F("\nnumInputs: "));
    Serial.print(config.macroConfig.numInputs);
    Serial.println(F("\ninputs:"));
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

void displayCurMode()
{
    oled.clear();
    if (macros.passwordMode)
    {
        oled.print(F("PASSWORD\nMODE"));
    }
    else
    {
        oled.print(F("MACRO@MODE"));
    }
}

bool Macros::readRfid()
{
    oled.clear();
    oled.print(F("RFID@KEY"));
    unsigned long time = millis();
    while (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    {
        if (millis() - time > 10000)
        {
            return false;
        }
    }
    for (uint8_t i = 0; i < 4; i++)
    {
        rfidUID[i] = rfid.uid.uidByte[i];
    }
    return true;
}
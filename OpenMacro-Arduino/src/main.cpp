#include <Arduino.h>
#include "Hardware.h"
#include "Macros.h"
#include "AESLib.h"

/*
    *Libraries used:
    SSD1306Ascii by greiman  (display driver) https://github.com/greiman/SSD1306Ascii
    IoAbstraction by davetcc (scheduling, buttons, encoders, IO expansion)  https://www.thecoderscorner.com/products/arduino-libraries/io-abstraction/
    MFRC522 by miguelbalboa  (RFID driver)
    HID-Project by NicoHood  (extends functionality of Arduino HID library)
    AESLib by davylandman    (AES password encryption)
*/

void receiveSerial_readAnalog()
{
    if (Serial && Serial.available())
    {
        oled.clear();
        oled.print(F("RECEIVING")); // the "F()" stuff around the string means we store the string in flash instead of SRAM

        if (macros.readFromSerial())
        {
            if (macros.config.isPassword)
            {
                if (macros.readRfid())
                {
                    uint8_t key[16];
                    for (uint8_t i = 0; i < 16; i++)
                    {
                        key[i] = macros.rfidUID[i % 4];
                    }
                    // encrypt password with UID
                    for (uint8_t i = 0; i < MAX_PASSWORDS; i++)
                    {
                        aes128_enc_single(key, macros.config.passwordConfig.passwords[i].str);
                    }
                    macros.saveToEEPROM(true);
                    oled.setCol(0);
                    oled.print(F("PASSWORD\nRECEIVED"));
                }
                else
                {
                    oled.setCol(0);
                    oled.print(F("RFID\nTIMEOUT"));
                }
                // read back macros
                macros.readFromEEPROM(false);
                taskManager.scheduleOnce(5000, displayCurMode);
            }
            else
            {
                macros.saveToEEPROM(false);
                oled.setCol(0);
                oled.print(F("MACRO\nRECEIVED"));
                // open 1200 magic baud to let GUI force reset
                Serial.begin(1200);
            }
        }
        else
        {
            oled.clear();
            oled.print(F("ERROR")); // the "@" here means space, cause we are using custom font to save space
            // roll back contents from eeprom
            if (!macros.readFromEEPROM(false))
            {
                macros.clearConfig();
            }
        }
        taskManager.scheduleOnce(5000, displayCurMode);
    }

    Gamepad.xAxis(map(analogRead(JOY_X), 1024, 0, INT16_MIN, INT16_MAX));
    Gamepad.yAxis(map(analogRead(JOY_Y), 1024, 0, INT16_MIN, INT16_MAX));
    Gamepad.write();
}

void setup()
{
    Serial.begin(9600);
    HardwareSetup();

    taskManager.scheduleFixedRate(5, receiveSerial_readAnalog);

    if (!macros.readFromEEPROM(false))
    {
        oled.print(F("NO@MACRO"));
    }
    else
    {
        oled.print(F("WELCOME"));
        uint8_t expanded = 0;
        if (expanded = macros.setupMacros())
        {
            oled.setCursor(0, 3);
            oled.print(expanded);
            oled.print(F("@EXPANDER"));
        }
    }
    taskManager.scheduleOnce(5000, displayCurMode);
}

void loop()
{
    taskManager.runLoop();
}
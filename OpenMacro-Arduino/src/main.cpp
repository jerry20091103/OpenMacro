#include <Arduino.h>
#include "Hardware.h"
#include "Macros.h"

/*
    *Libraries used:
    u8g2 by olikarus         (display driver) https://github.com/olikraus/u8g2/wiki
    IoAbstraction by davetcc (scheduling, buttons, encoders, IO expansion)  https://www.thecoderscorner.com/products/arduino-libraries/io-abstraction/
    MFRC522 by miguelbalboa  (RFID driver)
    HID-Project by NicoHood  (extends functionality of Arduino HID library)
*/

void receiveSerial()
{
    if (Serial && Serial.available())
    {
        u8x8.clear();
        u8x8.print(F("RECEIVING")); // the "F()" stuff around the string means we store the string in flash instead of SRAM
        int16_t bytes;

        bytes = macros.readFromSerial();
        if (bytes == sizeof(MacroConfig))
        {
            macros.saveToEEPROM();
            u8x8.setCursor(0, 0);
            u8x8.print(F("MACRO RECEIVED"));
            // open 1200 magic baud to let GUI force reset
            Serial.end();
            Serial.begin(1200);
        }
        else
        {
            u8x8.clear();
            u8x8.print(F("ERROR "));
            u8x8.print(bytes);
            // roll back contents from eeprom
            if (!macros.readFromEEPROM())
            {
                macros.clearConfig();
            }
        }
        taskManager.scheduleOnce(5000, displayCurMode);
    }
}

void readAnalog()
{
    Gamepad.xAxis(map(analogRead(JOY_X), 0, 1024, INT16_MIN, INT16_MAX));
    Gamepad.yAxis(map(analogRead(JOY_Y), 0, 1024, INT16_MIN, INT16_MAX));
    Gamepad.write();
}

void setup()
{
    Serial.begin(9600);
    HardwareSetup();

    taskManager.scheduleFixedRate(100, receiveSerial, TIME_MICROS);
    taskManager.scheduleFixedRate(5, readAnalog);

    if (!macros.readFromEEPROM())
    {
        u8x8.setCursor(0, 0);
        u8x8.print(F("NO MACRO CONFIG"));
    }
    else
    {
        u8x8.setCursor(0, 0);
        u8x8.print(F("WELCOME"));
        uint8_t expanded = 0;
        if (expanded = macros.setupMacros())
        {
            u8x8.setCursor(0, 18);
            u8x8.print(String(expanded));
            u8x8.print(F(" EXPANDER ADDED"));
        }
    }
    taskManager.scheduleOnce(5000, displayCurMode);

}

void loop()
{
    taskManager.runLoop();
}
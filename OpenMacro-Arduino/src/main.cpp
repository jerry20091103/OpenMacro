#include <Arduino.h>
#include "Hardware.h"
#include "Macros.h"

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

        int16_t bytes = macros.readFromSerial();
        if (bytes == sizeof(MacroConfig))
        {
            macros.saveToEEPROM(false);
            oled.setCol(0);
            oled.print(F("MACRO\nRECEIVED"));
            // open 1200 magic baud to let GUI force reset
            Serial.end();
            Serial.begin(1200);
        }
        else
        {
            oled.print(F("ERROR "));
            oled.print(bytes);
            // roll back contents from eeprom
            if (!macros.readFromEEPROM(false))
            {
                macros.clearConfig();
            }
        }
        taskManager.scheduleOnce(5000, displayCurMode);
    }

    Gamepad.xAxis(map(analogRead(JOY_X), 0, 1024, INT16_MIN, INT16_MAX));
    Gamepad.yAxis(map(analogRead(JOY_Y), 0, 1024, INT16_MIN, INT16_MAX));
    Gamepad.write();
}

void setup()
{
    Serial.begin(9600);
    HardwareSetup();

    taskManager.scheduleFixedRate(5, receiveSerial_readAnalog);

    if (!macros.readFromEEPROM(false))
    {
        oled.print(F("NO MACRO\nCONFIG"));
    }
    else
    {
        oled.print(F("WELCOME"));
        uint8_t expanded = 0;
        if (expanded = macros.setupMacros())
        {
            oled.setCursor(0, 3);
            oled.print(String(expanded));
            oled.print(F(" Expander"));
        }
    }
    taskManager.scheduleOnce(5000, displayCurMode);
}

void loop()
{
    taskManager.runLoop();
}
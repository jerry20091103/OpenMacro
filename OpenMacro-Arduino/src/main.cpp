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
*/

void receiveSerial()
{
    if (Serial && Serial.available())
    {
        oled.clear();
        oled.print(F("RECEIVING")); // the "F()" stuff around the string means we store the string in flash instead of SRAM

        int16_t bytes = macros.readFromSerial();
        if (bytes == sizeof(MacroConfig))
        {
            macros.saveToEEPROM();
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

    // !debug
    // test AES library
    char data[] = "0123456789012345"; // 16 chars == 16 bytes
    aes128_enc_single(macros.rfidUID, data);
    Serial.println(data);
    aes128_dec_single(macros.rfidUID, data);
    Serial.println(data);
}

void loop()
{
    taskManager.runLoop();
}
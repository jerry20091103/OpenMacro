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

void readRfid()
{
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial())
    {
        Serial.print("RFID:");

        u8x8.setCursor(0, 0);
        u8x8.print("RFID:");

        for (uint8_t i = 0; i < rfid.uid.size; i++)
        {
            Serial.print(rfid.uid.uidByte[i], HEX);
            Serial.print(" ");
            u8x8.print(rfid.uid.uidByte[i], HEX);
            u8x8.print(" ");
        }
        Serial.println();
    }
}

void receiveSerial()
{
    if (Serial && Serial.available())
    {
        u8x8.clear();
        u8x8.setCursor(0, 0);
        u8x8.print("RECEIVING");
        int16_t bytes;

        bytes = macros.readFromSerial();
        if (bytes == sizeof(MacroConfig))
        {
            macros.saveToEEPROM();
            u8x8.setCursor(0, 0);
            u8x8.print("MACRO RECEIVED");
        }
        else
        {
            u8x8.setCursor(0, 0);
            u8x8.print("ERROR" + String(bytes));
            // roll back contents from eeprom
            if (!macros.readFromEEPROM())
            {
                macros.clearConfig();
            }
        }
    }
}

void readAnalog()
{
    if (!macros.runningMacro)
    {
        // u8x8.setCursor(0, 38);
        // u8x8.print("             "); // overwrite old numbers
        // u8x8.setCursor(0, 38);
        // u8x8.print(String(analogRead(JOY_X)) + " " + String(analogRead(JOY_Y)));
    }
}

void setup()
{
    Serial.begin(9600);
    HardwareSetup();

    taskManager.scheduleFixedRate(100, readRfid);
    taskManager.scheduleFixedRate(10, receiveSerial, TIME_MICROS);
    taskManager.scheduleFixedRate(100, readAnalog);

    if (!macros.readFromEEPROM())
    {
        u8x8.setCursor(0, 0);
        u8x8.print("NO MACRO CONFIG");
    }
    else
    {
        u8x8.setCursor(0, 0);
        u8x8.print("WELCOME");
    }
}

void loop()
{
    taskManager.runLoop();
}
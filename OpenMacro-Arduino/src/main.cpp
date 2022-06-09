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
        Serial.println("RFID tag read:");
        Serial.print("UID: ");

        u8x8.setCursor(0, 0);
        u8x8.print("RFID:        ");
        u8x8.setCursor(0, 18);

        for (int i = 0; i < rfid.uid.size; i++)
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
        u8x8.setCursor(0, 0);
        u8x8.print("Receiving      ");
        int16_t bytes;

        bytes = macros.readFromSerial();
        u8x8.setCursor(0, 0);
        u8x8.print("Received " + String(bytes));
    }
}

void readAnalog()
{
    u8x8.setFont(u8x8_font_7x14_1x2_n);
    u8x8.setCursor(0, 38);
    u8x8.print("             "); // overwrite old numbers
    u8x8.setCursor(0, 38);
    u8x8.print(String(analogRead(JOY_X)) + " " + String(analogRead(JOY_Y)));
    u8x8.setFont(u8x8_font_8x13_1x2_r);
}

void setup()
{

    HardwareSetup();

    taskManager.scheduleFixedRate(100, readRfid);
    taskManager.scheduleFixedRate(10, receiveSerial, TIME_MICROS);
    taskManager.scheduleFixedRate(100, readAnalog);

    Serial.begin(9600);
    delay(1000);
}

void loop()
{
    taskManager.runLoop();
}
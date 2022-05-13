#include <Arduino.h>
#include "Hardware.h"

/*
    *Libraries used:
    u8g2 by olikarus         (display driver) https://github.com/olikraus/u8g2/wiki
    IoAbstraction by davetcc (scheduling, buttons, encoders, IO expansion)  https://www.thecoderscorner.com/products/arduino-libraries/io-abstraction/
    MFRC522 by miguelbalboa  (RFID driver)
*/

void readRfid()
{
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial())
    {
        Serial.println("RFID tag read:");
        Serial.print("UID: ");

        u8g2.clearBuffer();
        u8g2.setCursor(0, 20);
        u8g2.print("RFID:");
        u8g2.setCursor(0, 40);

        for (int i = 0; i < rfid.uid.size; i++)
        {
            Serial.print(rfid.uid.uidByte[i], HEX);
            Serial.print(" ");
            u8g2.print(rfid.uid.uidByte[i], HEX);
            u8g2.print(" ");
        }
        Serial.println();

        u8g2.sendBuffer();
    }
}

void receiveSerial()
{
    if (Serial.available())
    {
        uint8_t temp[10];
        Serial.readBytes(temp, 10);
        u8g2.clearBuffer();
        u8g2.setCursor(0, 20);
        for (int i = 0; i < 10; i++)
        {
            u8g2.print(temp[i]);
            u8g2.print(" ");
        }
        u8g2.sendBuffer();
    }
}

void readAnalog()
{
    u8g2.setCursor(0, 60);
    u8g2.setFont(u8g2_font_6x10_mn);
    u8g2.print("                    "); // overwrite old numbers
    u8g2.setCursor(0, 60);
    u8g2.print(analogRead(SLIDER0));
    u8g2.print(" ");
    u8g2.print(analogRead(SLIDER1));
    u8g2.print(" ");
    u8g2.print(analogRead(JOY_X));
    u8g2.print(" ");
    u8g2.print(analogRead(JOY_Y));
    u8g2.sendBuffer();

    u8g2.setFont(u8g2_font_10x20_mr);
}

void setup()
{
    Serial.begin(9600);

    //! debug: wait for serial monitor
    while (!Serial)
        ;

    HardwareSetup();

    taskManager.scheduleFixedRate(100, readRfid);
    taskManager.scheduleFixedRate(1, receiveSerial);
    taskManager.scheduleFixedRate(100, readAnalog);
}

void loop()
{
    taskManager.runLoop();
}
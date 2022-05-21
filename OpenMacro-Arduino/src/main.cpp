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
    if(Serial && Serial.available())
    {
        String temp;
        temp = Serial.readString();
        u8x8.setCursor(0, 0);
        u8x8.print(temp);
    }
}

void readAnalog()
{
    u8x8.setFont(u8x8_font_7x14_1x2_n );
    u8x8.setCursor(0, 38);
    u8x8.print("                    "); // overwrite old numbers
    u8x8.setCursor(0, 38);
    u8x8.print(String(analogRead(SLIDER0)) + " " + String(analogRead(SLIDER1)) + " " + String(analogRead(JOY_X)) + " " + String(analogRead(JOY_Y)));
    
    u8x8.setFont(u8x8_font_8x13_1x2_r );
}

void setup()
{
    Serial.begin(9600);

    //! debug: wait for serial monitor
    while (!Serial)
        ;

    HardwareSetup();

    taskManager.scheduleFixedRate(100, readRfid);
    taskManager.scheduleFixedRate(10, receiveSerial);
    //taskManager.scheduleFixedRate(100, readAnalog);
}

void loop()
{
    taskManager.runLoop();
}
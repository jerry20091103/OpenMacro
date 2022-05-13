#include "Controls.h"

void BtnPressCallback(pinid_t pin, bool isHeld)
{
    // we put serial here for testing
    Serial.println("Button Press: " + String(pin) + " isHeld: " + String(isHeld));

    u8g2.clearBuffer();
    u8g2.setCursor(0, 20);
    u8g2.print("Press:" + String(pin));
    u8g2.setCursor(0, 40);
    u8g2.print("isHeld: " + String(isHeld));
    u8g2.sendBuffer();
}

void BtnReleaseCallback(pinid_t pin, bool isHeld)
{
    Serial.println("Button Release: " + String(pin) + " isHeld: " + String(isHeld));

    u8g2.clearBuffer();
    u8g2.setCursor(0, 20);
    u8g2.println("Release:" + String(pin));
    u8g2.setCursor(0, 40);
    u8g2.println("isHeld: " + String(isHeld));
    u8g2.sendBuffer();
}

void EncCallback(int value)
{
    Serial.println("Encoder value: " + String(value));

    u8g2.clearBuffer();
    u8g2.setCursor(0, 20);
    u8g2.println("Encoder: " + String(value));
    u8g2.sendBuffer();
}


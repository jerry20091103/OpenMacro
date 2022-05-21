#include "Controls.h"

void BtnPressCallback(pinid_t pin, bool isHeld)
{
    // we put serial here for testing
    Serial.println("Button Press: " + String(pin) + " isHeld: " + String(isHeld));

    u8x8.setCursor(0, 0);
    u8x8.print("Press:" + String(pin) + "    ");
    u8x8.setCursor(0, 18);
    u8x8.print("isHeld: " + String(isHeld) + "    ");

    // !DEBUG
    // hold BTN0 to stop keyboard library in case of an bug
    if(pin == BTN0_PIN && isHeld)
    {
        Serial.println("EMERGENCY STOP");
        while (true) {}
    }
}

void BtnReleaseCallback(pinid_t pin, bool isHeld)
{
    Serial.println("Button Release: " + String(pin) + " isHeld: " + String(isHeld));

    u8x8.setCursor(0, 0);
    u8x8.println("Release:" + String(pin) + "    ");
    u8x8.setCursor(0, 18);
    u8x8.println("isHeld: " + String(isHeld));
}

void EncCallback(int value)
{
    Serial.println("Encoder value: " + String(value));

    u8x8.setCursor(0, 0);
    u8x8.println("Encoder: " + String(value) + "    ");
}


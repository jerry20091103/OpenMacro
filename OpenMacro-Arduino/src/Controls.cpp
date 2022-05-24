#include "Controls.h"
#include "Macros.h"

void BtnPressCallback(pinid_t pin, bool isHeld)
{
    macros.runMacro(getInputPinNum(pin));
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
    if(value > 0)
    {
        macros.runMacro(ENC_INC);
    }
    else if(value < 0)
    {
        macros.runMacro(ENC_DEC);
    }
    
    Serial.println("Encoder value: " + String(value));

    u8x8.setCursor(0, 0);
    u8x8.println("Encoder: " + String(value) + "    ");
}

// convert pin number to input number
uint8_t getInputPinNum(uint8_t hardwarePin)
{
    // expanded pins
    if (hardwarePin >= EXPANDER_PIN_OFFSET)
    {
        return hardwarePin - EXPANDER_PIN_OFFSET + NUM_BTN_INPUTS + 1;
    }
    // internal pins
    else
    {
        switch (hardwarePin)
        {
        case BTN0_PIN:
            return 0;
            break;
        case BTN1_PIN:
            return 1;
            break;
        case BTN2_PIN:
            return 2;
            break;
        case BTN3_PIN:
            return 3;
            break;
        case BTN4_PIN:
            return 4;
            break;
        case BTN5_PIN:
            return 5;
            break;
        case BTN6_PIN:
            return 6;
            break;
        case BTN7_PIN:
            return 7;
            break;
        case BTN8_PIN:
            return 8;
            break;
        case BTN_JOY_PIN:
            return BTN_JOY;
            break;

        default:
            break;
        }
    }
}


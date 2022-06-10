#include "Controls.h"
#include "Macros.h"

void BtnPressCallback(pinid_t pin, bool isHeld)
{
    // we put serial here for testing
    Serial.println("Button Press:" + String(pin));

    // !DEBUG
    // hold BTN1 to print config to serial
    if(pin == BTN1_PIN && isHeld)
    {
        macros.dumpConfig();
    }

    macros.runMacro(getInputPinNum(pin));
}

void EncCallback(int value)
{
    Serial.println("Encoder:" + String(value));


     if(value > 0)
    {
        macros.runMacro(ENC_INC);
    }
    else if(value < 0)
    {
        macros.runMacro(ENC_DEC);
    }
}

// convert pin number to input number
uint8_t getInputPinNum(uint8_t hardwarePin)
{
    // expanded pins
    if (hardwarePin >= EXPANDER_PIN_OFFSET)
    {
        return hardwarePin - EXPANDER_PIN_OFFSET + NUM_BTN_INPUTS;
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
            return 200;
            break;
        }
    }
}


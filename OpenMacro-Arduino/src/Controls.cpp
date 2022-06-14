#include "Controls.h"
#include "Macros.h"
#include "TaskManagerIO.h"
#include "ExecWithParameter.h"

void runMacroTask(uint8_t input)
{
    macros.runMacro(input);
}

void BtnPressCallback(pinid_t pin, bool isHeld)
{
    // we put serial here for testing
    // Serial.print(F("Button Press:"));
    // Serial.println(pin);

    // !DEBUG
    // hold BTN1 to print config to serial
    // if (pin == BTN1_PIN && isHeld)
    // {
    //     macros.dumpConfig();
    // }
    if (pin == BTN_ENC_PIN && isHeld)
    {
        oled.clear();
        oled.print(F("UPLOAD\nMACRO"));
        int16_t bytes = macros.sendToSerial();
        if(bytes != sizeof(MacroConfig))
        {
            oled.clear();
            oled.print(F("ERROR@"));
            oled.print(bytes);
        }
        taskManager.scheduleOnce(5000, displayCurMode);
    }
    else if (!isHeld)
    {
        auto task = new ExecWithParameter<uint8_t>(runMacroTask, getInputPinNum(pin));
        taskManager.execute(task, true);
    }
}

void BtnReleaseCallback(pinid_t pin, bool isHeld)
{
    if(pin == BTN_ENC_PIN && !isHeld)
    {
        macros.passwordMode = !macros.passwordMode;
        if (macros.passwordMode && !macros.readRfid())
        {
            macros.passwordMode = false;
            oled.setCol(0);
            oled.print(F("RFID\nTIMEOUT"));
            taskManager.scheduleOnce(5000, displayCurMode);
        }
        else
        {
            displayCurMode();
        }
    }
}

void EncCallback(int value)
{
    // Serial.print(F("Encoder:"));
    // Serial.println(value);

    if (value > 0)
    {
        macros.runMacro(ENC_DEC);
    }
    else if (value < 0)
    {
        macros.runMacro(ENC_INC);
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

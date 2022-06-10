#ifndef CONTROLS_H
#define CONTROLS_H

#include "Hardware.h"
#include "MacroConfig.h"

// handles user input with button and encoder callbacks
void BtnPressCallback(pinid_t pin, bool isHeld);
void EncCallback(int value);

// convert pin number to input number
uint8_t getInputPinNum(uint8_t hardwarePin);

#endif
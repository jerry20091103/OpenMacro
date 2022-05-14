#ifndef CONTROLS_H
#define CONTROLS_H

#include "Hardware.h"

// handles user input with button and encoder callbacks
void BtnPressCallback(pinid_t pin, bool isHeld);
void BtnReleaseCallback(pinid_t pin, bool isHeld);
void EncCallback(int value);

class KeyMatrixListener : public KeyboardListener
{
public:
    void keyPressed(char key, bool held)
    {
        BtnPressCallback(key-49, held);
    }
    void keyReleased(char key)
    {
        BtnReleaseCallback(key-49, false);
    }
};

#endif
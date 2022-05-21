#ifndef MACRO_CONFIG_H
#define MACRO_CONFIG_H

#include "HID-Project.h"

#define NUM_BTN_INPUTS 12
#define MAX_PRESETS 9

enum MacroMode : uint8_t
{
    KEYBAORD_MOUSE_CLICK,
    MOUSE_MOVE,
};

enum ButtonInputs : uint8_t
{
    // 3x3 buttons
    BTN0,
    BTN1,
    BTN2,
    BTN3,
    BTN4,
    BTN5,
    BTN6,
    BTN7,
    BTN8,
    // joystick button
    BTN_JOY,
    // encoder turn
    ENC_INC,
    ENC_DEC
    // encoder button is reserved for switching to password mode.
};

struct ButtonAction
{
    uint8_t size;
    uint16_t delay;
    uint8_t *btnPress[3]; 
    // keycodes defined in ImprovedKeylayouts.h, mouse button defined in MouseAPI.hpp.
    // each button press has 3 values : [modifier keycode, keycode, mouse btn]
    // the struct contains list of button press (btnPress[size][3])
    // the "delay" stores the time interval between each key presses.
};

struct MouseAction
{
    int16_t mouseMove[3]; // stores [mouseX, mouseY, wheel] (relative mouse move)
};

struct MacroAction
{
    MacroMode mode;
    union data
    {
        ButtonAction btn;
        MouseAction mouse;
    };  
};



struct MacroPreset
{   
    uint8_t size;
    MacroAction *inputs; 
    // a list of button inputs, ordered as the "ButtonInputs" enum above.
    // dynamic length to allow further expansion.
};

struct MacroConfig
{
    MacroPreset presets[9];
};


#endif
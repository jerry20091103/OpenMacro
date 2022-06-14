#ifndef MACRO_CONFIG_H
#define MACRO_CONFIG_H

#define NUM_BTN_INPUTS 12
#define MAX_PRESETS 9
#define MAX_PASSWORDS 9
#define MAX_PASSWORD_LEN 16
#define MAX_EXPANDERS 2
#define MAX_INPUTS (NUM_BTN_INPUTS + MAX_EXPANDERS * 16)
#define COMMAND_BUF_SIZE 512

#include <stdint.h>
#include <string.h>

enum MacroMode : uint8_t
{
    KEYBOARD_MOUSE_CLICK,
    MOUSE_MOVE,
};

// a total of 12 internal inputs
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

struct MacroPassword
{
    uint8_t size;
    char str[MAX_PASSWORD_LEN];
};

struct MacroPacket
{
    MacroMode mode;
    uint8_t modifierCode;
    union
    {
        uint8_t keycode;
        struct __attribute__ ((packed)) {
            uint8_t mouseBtn;
            int8_t mouseX;
            int8_t mouseY;
            int8_t wheel;
            // mouseBtn defaults to click drag, for click only, set mouseX, Y to 0.
        } mouseMove;
    };
    // keycodes and mouse buttons defined in Keycodes.h.
    // "modifierCode" key is pressed throughout the whole MacroPacket.
    // for MOUSE_MOVE mode:
    //      MouseMove struct (relative mouse move)
    // for KEYBAORD_MOUSE_CLICK mode:
    //      uint8_t keycode
};

struct __attribute__ ((packed)) MacroAction
{
    uint8_t size;
    uint16_t delay;
    uint16_t data;
    // "data" points to the first MacroPacket in commandBuffer (commandBuffer[data]), where a list of MacroPacket is stored one after another.(tightly packed)
    // "delay" stores the time interval between each key presses / mouse move
    // "size" number of macro packets in the commandBuffer
};

struct MacroConfig
{
    uint8_t expanderAddr[MAX_EXPANDERS];
    uint8_t numInputs;
    MacroAction inputs[MAX_INPUTS];
    char commandBuffer[COMMAND_BUF_SIZE];
    // Each IO expander provides additional 16 pins.
    // We plan to use MCP23017 I2C expander IC.
    // Expanded pins starts from 13 (ex. expander 1: 13 ~ 29; expander 2: 30 ~ 46)
    // "expandeAddr" stores the 8bit address for each IO expander
    // "numInputs" is the total number of input pins used.
    // "inputs" is an array of button inputs, ordered as the "ButtonInputs" enum above.
    // Expanded pin comes right after internal pins (starting from inputs[13])
};

struct PasswordConfig
{
    MacroPassword passwords[MAX_PASSWORDS];
};

struct __attribute__ ((packed)) Config
{
    uint8_t isPassword;
    // 0 -> macro, other -> password
    union 
    {
        MacroConfig macroConfig;
        PasswordConfig passwordConfig;
    };
    
};

#endif

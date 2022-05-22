#ifndef MACRO_CONFIG_H
#define MACRO_CONFIG_H

#define NUM_BTN_INPUTS 12
#define MAX_PRESETS 9
#define MAX_PASSWORDS 9
#define MAX_PASSWORD_LEN 20
#define MAX_EXPANDERS 2
#define COMMAND_BUF_SIZE 1000

#include <stdint.h>
#include <string.h>

enum MacroMode : uint8_t
{
    KEYBAORD_MOUSE_CLICK,
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
    uint16_t size;
    char str[MAX_PASSWORD_LEN];
};

struct MacroAction
{
    MacroMode mode;
    uint8_t size;
    uint16_t delay;
    char *data;
    // keycodes defined in ImprovedKeylayouts.h, mouse button defined in MouseAPI.hpp.
    // for MOUSE_MOVE mode:
    //      int16_t[3]  -> [mouseX, mouseY, wheel] (relative mouse move)
    // for KEYBAORD_MOUSE_CLICK mode:
    //      uint8_t[3]  -> [modifier keycode, keycode, mouse btn]
    // "data" points to the first action in commandBuffer, where a list of button press or mouse moves is stored one after another.
    // "delay" stores the time interval between each key presses / mouse move
    // "size" is the size of "data" in chars.
};

struct MacroPreset
{
    uint8_t numInputs;
    MacroAction *inputs;
    // a list of button inputs, ordered as the "ButtonInputs" enum above.
    // "numInputs" is the total number of input pins used.
    // "inputs" points to the first MacroAction (aka BTN0's macro)
    // Expanded pin comes right after internal pins (starting from inputs[13])
};

struct MacroConfig
{
    uint8_t expanderAddr[MAX_EXPANDERS];
    MacroPreset presets[MAX_PRESETS];
    MacroPassword passwords[MAX_PASSWORDS];
    char commamdBuffer[COMMAND_BUF_SIZE];
    // Each IO expander provides additional 16 pins.
    // We plan to use MCP23017 I2C expander IC.
    // Expanded pins starts from 13 (ex. expander 1: 13 ~ 29; expander 2: 30 ~ 46)
    // "expandeAddr" stores the 8bit address for each IO expander
};

void SerializeConfig(MacroConfig *config, char *data)
{
    memcpy(data, config, sizeof(MacroConfig));
}

void DeserializeConfig(char *data, MacroConfig *config)
{
    memcpy(config, data, sizeof(MacroConfig));
}

#endif

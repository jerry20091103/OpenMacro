#ifndef MACRO_CONFIG_H
#define MACRO_CONFIG_H

#define NUM_BTN_INPUTS 12
#define MAX_PRESETS 9
#define MAX_PASSWORDS 9

#include <stdint.h>

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

struct MacroPassword
{
    uint16_t size;
    char *str;
};

struct MacroAction
{
    MacroMode mode;
    uint8_t size;
    uint16_t delay;
    int16_t mouseMove[3];
    uint8_t *btnPress[3];
    // keycodes defined in ImprovedKeylayouts.h, mouse button defined in MouseAPI.hpp.
    // each button press has 3 values : [modifier keycode, keycode, mouse btn]
    // the struct contains list of button press (btnPress[size][3])
    // the "delay" stores the time interval between each key presses.
    // mouseMove stores [mouseX, mouseY, wheel] (relative mouse move)
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
    MacroPreset presets[MAX_PRESETS];
    MacroPassword passwords[MAX_PASSWORDS];
};

void SerializeConfig(MacroConfig *config, char *data)
{
    uint8_t *p8 = (uint8_t *)data;
    // presets
    for (int i = 0; i < MAX_PRESETS; i++)
    {
        *p8 = config->presets[i].size;
        p8++;
        for (int input = 0; i < config->presets[i].size; i++)
        {
            *p8 = config->presets[i].inputs[input].mode;
            p8++;
            *p8 = config->presets[i].inputs[input].size;
            p8++;

            uint16_t *p_uint16_t = (uint16_t *)p8;
            *p_uint16_t = config->presets[i].inputs[input].delay;
            p_uint16_t++;

            int16_t *p_int16_t = (int16_t *)p_uint16_t;
            *p_int16_t = config->presets[i].inputs[input].mouseMove[0];
            p_int16_t++;
            *p_int16_t = config->presets[i].inputs[input].mouseMove[1];
            p_int16_t++;
            *p_int16_t = config->presets[i].inputs[input].mouseMove[2];
            p_int16_t++;

            p8 = (uint8_t *)p_int16_t;
            for (int press = 0; press < config->presets[i].inputs[input].size * 3; i++)
            {
                *p8 = *config->presets[i].inputs[input].btnPress[press];
                p8++;
            }
        }
    }
    // passwords
    for (int i = 0; i < MAX_PASSWORDS; i++)
    {
        *p8 = config->passwords[i].size;
        p8++;

        char *p_char = (char *)p8;
        for (int len = 0; len < config->passwords[i].size; len++)
        {
            *p_char = config->passwords->str[len];
            p_char++;
        }
    }
}

void DeserializeConfig(char *data, MacroConfig *config)
{
    uint8_t *p8 = (uint8_t *)data;
    // presets
    for (int i = 0; i < MAX_PRESETS; i++)
    {
        config->presets[i].size = *p8;
        p8++;

        config->presets[i].inputs = new MacroAction[config->presets[i].size];

        for (int input = 0; i < config->presets[i].size; i++)
        {
            config->presets[i].inputs[input].mode = (MacroMode)*p8;
            p8++;
            config->presets[i].inputs[input].size = *p8;
            p8++;

            uint16_t *p_uint16_t = (uint16_t *)p8;
            config->presets[i].inputs[input].delay = *p_uint16_t;
            p_uint16_t++;

            int16_t *p_int16_t = (int16_t *)p_uint16_t;
            config->presets[i].inputs[input].mouseMove[0] = *p_uint16_t;
            p_int16_t++;
            config->presets[i].inputs[input].mouseMove[1] = *p_uint16_t;
            p_int16_t++;
            config->presets[i].inputs[input].mouseMove[2] = *p_uint16_t;
            p_int16_t++;

            p8 = (uint8_t *)p_int16_t;

            config->presets[i].inputs[input].btnPress[0] = new uint8_t[config->presets[i].inputs[input].size * 3];

            for (int press = 0; press < config->presets[i].inputs[input].size * 3; i++)
            {
                *config->presets[i].inputs[input].btnPress[press] = *p8;
                p8++;
            }
        }
    }
    // passwords
    for (int i = 0; i < MAX_PASSWORDS; i++)
    {
        config->passwords[i].size = *p8;
        p8++;

        char *p_char = (char *)p8;

        config->passwords[i].str = new char[config->passwords[i].size];

        for (int len = 0; len < config->passwords[i].size; len++)
        {
            *p_char = config->passwords->str[len];
            p_char++;
        }
    }
}

#endif
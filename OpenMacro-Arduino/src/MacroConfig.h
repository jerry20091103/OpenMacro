#ifndef MACRO_CONFIG_H
#define MACRO_CONFIG_H

#define NUM_BTN_INPUTS 12
#define MAX_PRESETS 9
#define MAX_PASSWORDS 9

enum MacroMode : unsigned char
{
    KEYBAORD_MOUSE_CLICK,
    MOUSE_MOVE,
};

enum ButtonInputs : unsigned char
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
    unsigned short size;
    char *str;
};

struct MacroAction
{
    MacroMode mode;
    unsigned char size;
    unsigned short delay;
    short mouseMove[3];
    unsigned char *btnPress[3];
    // keycodes defined in ImprovedKeylayouts.h, mouse button defined in MouseAPI.hpp.
    // each button press has 3 values : [modifier keycode, keycode, mouse btn]
    // the struct contains list of button press (btnPress[size][3])
    // the "delay" stores the time interval between each key presses.
    // mouseMove stores [mouseX, mouseY, wheel] (relative mouse move)
};

struct MacroPreset
{
    unsigned char size;
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
    unsigned char *p8 = (unsigned char *)data;
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

            unsigned short *p_ushort = (unsigned short *)p8;
            *p_ushort = config->presets[i].inputs[input].delay;
            p_ushort++;

            short *p_short = (short *)p_ushort;
            *p_short = config->presets[i].inputs[input].mouseMove[0];
            p_short++;
            *p_short = config->presets[i].inputs[input].mouseMove[1];
            p_short++;
            *p_short = config->presets[i].inputs[input].mouseMove[2];
            p_short++;

            p8 = (unsigned char *)p_short;
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
    unsigned char *p8 = (unsigned char *)data;
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

            unsigned short *p_ushort = (unsigned short *)p8;
            config->presets[i].inputs[input].delay = *p_ushort;
            p_ushort++;

            short *p_short = (short *)p_ushort;
            config->presets[i].inputs[input].mouseMove[0] = *p_ushort;
            p_short++;
            config->presets[i].inputs[input].mouseMove[1] = *p_ushort;
            p_short++;
            config->presets[i].inputs[input].mouseMove[2] = *p_ushort;
            p_short++;

            p8 = (unsigned char *)p_short;

            config->presets[i].inputs[input].btnPress[0] = new unsigned char[config->presets[i].inputs[input].size * 3];

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
#ifndef MACROS_H
#define MACROS_H

#include "MacroConfig.h"
#include <EEPROM.h>

class Macros
{
private:
    MacroConfig config;
public:
    int16_t readFromSerial();
    bool sendToSerial();
    void saveToEEPROM();
    void readFromEEPROM();
    void setupMacros();

    void runMacro(uint8_t input);
};

extern Macros macros;


#endif
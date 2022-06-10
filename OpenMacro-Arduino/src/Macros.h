#ifndef MACROS_H
#define MACROS_H

#include "MacroConfig.h"
#include <EEPROM.h>

class Macros
{
private:
    MacroConfig config;
    void setupMacros();
public:
    int16_t readFromSerial();
    bool sendToSerial();
    void saveToEEPROM();
    bool readFromEEPROM();
    void clearConfig();
    void dumpConfig();


    void runMacro(uint8_t input);
};

extern Macros macros;


#endif
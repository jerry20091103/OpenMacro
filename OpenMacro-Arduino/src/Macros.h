#ifndef MACROS_H
#define MACROS_H

#include "MacroConfig.h"
#include <EEPROM.h>

class Macros
{
private:
    Config config;
    
public:
    int16_t readFromSerial();
    bool sendToSerial();
    void saveToEEPROM();
    bool readFromEEPROM();
    void clearConfig();
    void dumpConfig();
    void runMacro(uint8_t input);
    uint8_t setupMacros();
    bool checkExpanders();

    bool runningMacro = false;
};

extern Macros macros;


#endif
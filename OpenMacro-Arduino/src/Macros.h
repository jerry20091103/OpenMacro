#ifndef MACROS_H
#define MACROS_H

#include "MacroConfig.h"
#include <EEPROM.h>

class Macros
{
private:
    
    
public:
    bool readFromSerial();
    bool sendToSerial();
    void saveToEEPROM(bool isPassword);
    bool readFromEEPROM(bool isPassword);
    void clearConfig();
    void dumpConfig();
    void runMacro(uint8_t input);
    void releaseMacro(uint8_t input);
    uint8_t setupMacros();
    bool checkExpanders();
    bool readRfid();

    Config config;
    bool passwordMode = false;
    uint8_t rfidUID[4];
};

void displayCurMode();

extern Macros macros;


#endif
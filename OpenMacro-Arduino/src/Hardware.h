#ifndef HARDWARE_H
#define HARDWARE_H

#include "IoAbstractionWire.h"
#include "TaskManagerIO.h"
#include "MFRC522.h"
#include "u8g2lib.h"
#include "Keyboard.h"
#include <KeyboardManager.h>

// *Pins
// Rotray Encoders and buttons
#define ENCA 7
#define ENCB 12
#define BTN_ENC 22

#define COL0 9
#define COL1 8
#define COL2 6
#define ROW0 5
#define ROW1 4
#define ROW2 11

// button number for key matrix
#define BTN0 0
#define BTN1 1
#define BTN2 2
#define BTN3 3
#define BTN4 4
#define BTN5 5
#define BTN6 6
#define BTN7 7
#define BTN8 8

// sliders
#define SLIDER0 A0
#define SLIDER1 A1

// joysticks
#define JOY_X A5
#define JOY_Y A3
#define BTN_JOY 20

// rfid
#define RFID_SS 10
#define RFID_RST 17

// display
#define DISPLAY_ADD 0x3c

// rotary encoder object
extern HardwareRotaryEncoder* enc0;

// RFID reader object
extern MFRC522 rfid;

// OLED display object
extern U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8;

void HardwareSetup();

#endif
#ifndef HARDWARE_H
#define HARDWARE_H

#include "IoAbstractionWire.h"
#include "TaskManagerIO.h"
#include "MFRC522.h"
#include "u8g2lib.h"
#include "Keyboard.h"

// *Pins
// Rotray Encoders and buttons
#define ENCA 11
#define ENCB 13
#define BTN_ENC 22

#define BTN0 12
#define BTN1 9
#define BTN2 8
#define BTN3 7
#define BTN4 6
#define BTN5 5
#define BTN6 4
#define BTN7 0 
#define BTN8 1

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
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

void HardwareSetup();

#endif
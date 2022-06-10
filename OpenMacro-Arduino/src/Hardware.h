#ifndef HARDWARE_H
#define HARDWARE_H

#include "IoAbstractionWire.h"
#include "TaskManagerIO.h"
#include "MFRC522.h"
#include "u8g2lib.h"
#include "HID-Project.h"
#include <EEPROM.h>
#include <Wire.h>

// *Pins
// Rotray Encoders and buttons
#define ENCA 7
#define ENCB 12
#define BTN_ENC_PIN 22

// buttons
#define BTN0_PIN 9
#define BTN1_PIN 8
#define BTN2_PIN 6
#define BTN3_PIN 5
#define BTN4_PIN 4
#define BTN5_PIN 18
#define BTN6_PIN 19
#define BTN7_PIN 13
#define BTN8_PIN 11

// joysticks
#define JOY_X A5
#define JOY_Y A3
#define BTN_JOY_PIN 20

// rfid
#define RFID_SS 10
#define RFID_RST 17

// display
#define DISPLAY_ADD 0x3c

// expanders
#define EXPANDER_PIN_OFFSET 30 
// *pin numbers starting from 30 are external pins from expanders.

// rotary encoder object
extern HardwareRotaryEncoder* enc0;

// RFID reader object
extern MFRC522 rfid;

// OLED display object
extern U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8;

extern MultiIoAbstractionRef multiIo;

void HardwareSetup();

#endif
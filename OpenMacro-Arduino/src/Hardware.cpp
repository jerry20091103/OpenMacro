#include "Hardware.h"
#include "Controls.h"
#include "font5x7_u.h"

HardwareRotaryEncoder *enc0;
MFRC522 rfid;
SSD1306AsciiWire oled;

// io abstraction object
MultiIoAbstractionRef multiIo = multiIoExpander(EXPANDER_PIN_OFFSET);

void HardwareSetup()
{
    Wire.begin();
    // We use IOAbstraction library to handle switches and rotary encoders.
    // It also has built task manager for easy scheduling.
    // With the help of this library, we can also add IO pins with I2C expanders and can use them as if they were original arduino pins.
    // https://www.thecoderscorner.com/products/arduino-libraries/io-abstraction/
    switches.init(multiIo, SWITCHES_POLL_KEYS_ONLY, true);
    //* setup rotray encoder
    enc0 = new HardwareRotaryEncoder(ENCA, ENCB, EncCallback, HWACCEL_SLOWER);
    // add encoder to the switches object in the library
    switches.setEncoder(0, enc0);
    // set how the encoders operate
    enc0->changePrecision(0, 0);
    //* setup buttons
    switches.addSwitch(BTN_ENC_PIN, BtnPressCallback);
    switches.addSwitch(BTN_JOY_PIN, BtnPressCallback);
    switches.addSwitch(BTN0_PIN, BtnPressCallback);
    switches.addSwitch(BTN1_PIN, BtnPressCallback);
    switches.addSwitch(BTN2_PIN, BtnPressCallback);
    switches.addSwitch(BTN3_PIN, BtnPressCallback);
    switches.addSwitch(BTN4_PIN, BtnPressCallback);
    switches.addSwitch(BTN5_PIN, BtnPressCallback);
    switches.addSwitch(BTN6_PIN, BtnPressCallback);
    switches.addSwitch(BTN7_PIN, BtnPressCallback);
    switches.addSwitch(BTN8_PIN, BtnPressCallback);
    switches.onRelease(BTN_ENC_PIN, BtnReleaseCallback);
    // multiIoAddExpander(multiIo, ioFrom23017(0x20), 16);
    // switches.addSwitch(EXPANDER_PIN_OFFSET, BtnPressCallback);
    // switches.addSwitch(EXPANDER_PIN_OFFSET + 1, BtnPressCallback);
    // switches.addSwitch(EXPANDER_PIN_OFFSET + 2, BtnPressCallback);
    // switches.addSwitch(EXPANDER_PIN_OFFSET + 3, BtnPressCallback);

    //* setup display
    oled.begin(&Adafruit128x64, DISPLAY_ADD);
    oled.setFont(font5x7_u); // this is a custom font with only numbbers and UPPER case characters, use "@" for space
    oled.set2X();
    oled.clear();

    //* setup RFID
    SPI.begin();
    rfid.PCD_Init(RFID_SS, RFID_RST);

    //*  start keyboard library
    Keyboard.begin();
    Mouse.begin();
    Gamepad.begin();
    Consumer.begin();
}
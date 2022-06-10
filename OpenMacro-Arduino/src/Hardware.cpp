#include "Hardware.h"
#include "Controls.h"

HardwareRotaryEncoder *enc0;
MFRC522 rfid;
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8;

// io abstraction object
MultiIoAbstractionRef multiIo = multiIoExpander(EXPANDER_PIN_OFFSET);

void HardwareSetup()
{
    // We use IOAbstraction library to handle switches and rotary encoders.
    // It also has built task manager for easy scheduling.
    // With the help of this library, we can also add IO pins with I2C expanders and can use them as if they were original arduino pins.
    // https://www.thecoderscorner.com/products/arduino-libraries/io-abstraction/
    multiIoAddExpander(multiIo, ioFrom23017(0x20), 16);
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

    //* setup joystick
    ioDevicePinMode(ioUsingArduino(), JOY_X, INPUT);
    ioDevicePinMode(ioUsingArduino(), JOY_Y, INPUT);

    //* setup display
    u8x8.print(" "); // Somehow this is required or the display won't work...
    u8x8.begin();
    u8x8.clear();
    u8x8.setFont(u8x8_font_victoriamedium8_u); // choose a suitable font

    //* setup RFID
    SPI.begin();
    rfid.PCD_Init(RFID_SS, RFID_RST);

    //*  start keyboard library
    Keyboard.begin();
    Mouse.begin();
    Gamepad.begin();
}
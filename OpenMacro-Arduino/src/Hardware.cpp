#include "Hardware.h"
#include "Controls.h"

HardwareRotaryEncoder *enc0;
MFRC522 rfid;
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8;

void HardwareSetup()
{
    // We use IOAbstraction library to handle switches and rotary encoders.
    // It also has built task manager for easy scheduling.
    // With the help of this library, we can also add IO pins with I2C expanders and can use them as if they were original arduino pins.
    // https://www.thecoderscorner.com/products/arduino-libraries/io-abstraction/

    switches.init(ioUsingArduino(), SWITCHES_POLL_KEYS_ONLY, true);
    
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

    // addSwitch() only binds a callback for button press, we need to bind button release callback separetely.
    switches.onRelease(BTN_ENC_PIN, BtnReleaseCallback);
    switches.onRelease(BTN_JOY_PIN, BtnReleaseCallback);
    switches.onRelease(BTN0_PIN, BtnReleaseCallback);
    switches.onRelease(BTN1_PIN, BtnReleaseCallback);
    switches.onRelease(BTN2_PIN, BtnReleaseCallback);
    switches.onRelease(BTN3_PIN, BtnReleaseCallback);
    switches.onRelease(BTN4_PIN, BtnReleaseCallback);
    switches.onRelease(BTN5_PIN, BtnReleaseCallback);
    switches.onRelease(BTN6_PIN, BtnReleaseCallback);
    switches.onRelease(BTN7_PIN, BtnReleaseCallback);
    switches.onRelease(BTN8_PIN, BtnReleaseCallback);

    //* setup joystick
    ioDevicePinMode(ioUsingArduino(), JOY_X, INPUT);
    ioDevicePinMode(ioUsingArduino(), JOY_Y, INPUT);

    //* setup display
    u8x8.begin();
    u8x8.clear();
    u8x8.setFont(u8x8_font_8x13_1x2_r); // choose a suitable font
    u8x8.setCursor(0, 0);               // set write position
    u8x8.print("hello world");      // write something to the internal memory

    //* setup RFID
    SPI.begin();
    rfid.PCD_Init(RFID_SS, RFID_RST);
    Serial.print("RFID reader :");
    rfid.PCD_DumpVersionToSerial();
    Serial.println();

    //*  start keyboard library
    Keyboard.begin();
    Mouse.begin();
    Gamepad.begin();
}
#include "Hardware.h"
#include "Controls.h"

HardwareRotaryEncoder *enc0;
MFRC522 rfid;
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);

void HardwareSetup()
{
    // We use IOAbstraction library to handle switches and rotary encoders.
    // It also has built task manager for easy scheduling.
    // With the help of this library, we can also add IO pins with I2C expanders and can use them as if they were original arduino pins.
    // https://www.thecoderscorner.com/products/arduino-libraries/io-abstraction/

    switches.init(ioUsingArduino(), SWITCHES_POLL_EVERYTHING, true);
    
    //* setup rotray encoder
    enc0 = new HardwareRotaryEncoder(ENCA, ENCB, EncCallback, HWACCEL_SLOWER);
    // add encoder to the switches object in the library
    switches.setEncoder(0, enc0);
    // set how the encoders operate
    enc0->changePrecision(0, 0);

    //* setup buttons
    switches.addSwitch(BTN_ENC, BtnPressCallback);
    switches.addSwitch(BTN_JOY, BtnPressCallback);
    switches.addSwitch(BTN0, BtnPressCallback);
    switches.addSwitch(BTN1, BtnPressCallback);
    switches.addSwitch(BTN2, BtnPressCallback);
    switches.addSwitch(BTN3, BtnPressCallback);
    switches.addSwitch(BTN4, BtnPressCallback);
    switches.addSwitch(BTN5, BtnPressCallback);
    switches.addSwitch(BTN6, BtnPressCallback);
    // switches.addSwitch(BTN7, BtnPressCallback);
    // switches.addSwitch(BTN8, BtnPressCallback);

    // addSwitch() only binds a callback for button press, we need to bind button release callback separetely.
    switches.onRelease(BTN_ENC, BtnReleaseCallback);
    switches.onRelease(BTN_JOY, BtnReleaseCallback);
    switches.onRelease(BTN0, BtnReleaseCallback);
    switches.onRelease(BTN1, BtnReleaseCallback);
    switches.onRelease(BTN2, BtnReleaseCallback);
    switches.onRelease(BTN3, BtnReleaseCallback);
    switches.onRelease(BTN4, BtnReleaseCallback);
    switches.onRelease(BTN5, BtnReleaseCallback);
    switches.onRelease(BTN6, BtnReleaseCallback);
    // switches.onRelease(BTN7, BtnReleaseCallback);
    // switches.onRelease(BTN8, BtnReleaseCallback);

    //* setup sliders and joystick
    ioDevicePinMode(ioUsingArduino(), SLIDER0, INPUT);
    ioDevicePinMode(ioUsingArduino(), SLIDER1, INPUT);
    ioDevicePinMode(ioUsingArduino(), JOY_X, INPUT);
    ioDevicePinMode(ioUsingArduino(), JOY_Y, INPUT);

    //* setup display
    // TODO
    u8g2.begin();
    u8g2.clearBuffer();
    u8g2.setFontMode(0);
    u8g2.setFont(u8g2_font_10x20_mr); // choose a suitable font
    u8g2.setCursor(0, 20);               // set write position
    u8g2.print("hello world");      // write something to the internal memory
    u8g2.sendBuffer();

    //* setup RFID
    // TODO
    SPI.begin();
    rfid.PCD_Init(RFID_SS, RFID_RST);
    Serial.print("RFID reader :");
    rfid.PCD_DumpVersionToSerial();
    Serial.println();
}
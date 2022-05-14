#include "Hardware.h"
#include "Controls.h"

HardwareRotaryEncoder *enc0;
MFRC522 rfid;
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8;

const char pgmLayout[] PROGMEM = "123456789";
KeyboardLayout layout(3, 3, pgmLayout);
MatrixKeyboardManager keyboard;
KeyMatrixListener myKeyMatrixListener;

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
    switches.addSwitch(BTN_ENC, BtnPressCallback);
    switches.addSwitch(BTN_JOY, BtnPressCallback);

    // addSwitch() only binds a callback for button press, we need to bind button release callback separetely.
    switches.onRelease(BTN_ENC, BtnReleaseCallback);
    switches.onRelease(BTN_JOY, BtnReleaseCallback);

    //* setup key matrix
    layout.setRowPin(0, ROW0);
    layout.setRowPin(1, ROW1);
    layout.setRowPin(2, ROW2);
    layout.setColPin(0, COL0);
    layout.setColPin(1, COL1);
    layout.setColPin(2, COL2);
    keyboard.initialise(ioUsingArduino(), &layout, &myKeyMatrixListener);

    //* setup sliders and joystick
    ioDevicePinMode(ioUsingArduino(), SLIDER0, INPUT);
    ioDevicePinMode(ioUsingArduino(), SLIDER1, INPUT);
    ioDevicePinMode(ioUsingArduino(), JOY_X, INPUT);
    ioDevicePinMode(ioUsingArduino(), JOY_Y, INPUT);

    //* setup display
    // TODO
    u8x8.begin();
    u8x8.clear();
    u8x8.setFont(u8x8_font_8x13_1x2_r); // choose a suitable font
    u8x8.setCursor(0, 0);               // set write position
    u8x8.print("hello world");      // write something to the internal memory

    //* setup RFID
    // TODO
    SPI.begin();
    rfid.PCD_Init(RFID_SS, RFID_RST);
    Serial.print("RFID reader :");
    rfid.PCD_DumpVersionToSerial();
    Serial.println();
}
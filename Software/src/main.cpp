// Include
#include <Arduino.h>
#include <Control_Surface.h>
#include <LiquidCrystal_I2C.h>

// Variables
uint8_t lastKnobVal[2] = {255, 255};
bool lastBtnState[4] = {};
uint32_t lastUpdate = 0;

// Lib setup
USBMIDI_Interface midi;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Controls
CCPotentiometer knobs[] = {
    { 3, {MIDI_CC::Sound_Controller_1, Channel_1} },
    { 4, {MIDI_CC::Sound_Controller_2, Channel_1} },
};

CCButton buttons[] = {
    { 16, {MIDI_CC::General_Purpose_Controller_1, Channel_1} },
    { 17, {MIDI_CC::General_Purpose_Controller_2, Channel_1} },
    {  7, {MIDI_CC::General_Purpose_Controller_3, Channel_1} },
    { 15, {MIDI_CC::General_Purpose_Controller_4, Channel_1} },
};

// MIDI output indicator
NoteLED leds[] = {
   { 13, {MIDI_Notes::C(4), Channel_1}},
   { 13, {MIDI_Notes::D(4), Channel_1}},
};

CCButtonLatched ccLEDs[] = {
   { 13, {MIDI_CC::General_Purpose_Controller_5, Channel_1}},
   { 13, {MIDI_CC::General_Purpose_Controller_6, Channel_1}},
};

void updateDisplay() {
    uint8_t k0 = knobs[0].getValue();
    uint8_t k1 = knobs[1].getValue();

    bool btns[4];
    bool btnChanged = false;
    for (uint8_t i = 0; i < 4; i++) {
        btns[i] = buttons[i].getButtonState() == Button::Pressed;
        if (btns[i] != lastBtnState[i]) btnChanged = true;
    }

    bool knobChanged = (k0 != lastKnobVal[0] || k1 != lastKnobVal[1]);
    if (!knobChanged && !btnChanged) return; // nothing to redraw

    // Update tracked state
    lastKnobVal[0] = k0;
    lastKnobVal[1] = k1;
    for (uint8_t i = 0; i < 4; i++) lastBtnState[i] = btns[i];

    // Row 0: knobs
    lcd.setCursor(0, 0);
    lcd.print("K1:");
    lcd.print(k0);
    lcd.print("  K2:");
    lcd.print(k1);
    lcd.print("   ");

    // Row 1: buttons
    lcd.setCursor(0, 1);
    for (uint8_t i = 0; i < 4; i++) {
        lcd.print("B");
        lcd.print(i + 1);
        lcd.print("");
        lcd.print(btns[i] ? "\xFF " : "_ ");
    }
}

void setup() {
    Wire.begin(10, 11);
    lcd.init();
    lcd.backlight();

    lcd.setCursor(0, 0);
    lcd.print("Control Surface");
    lcd.setCursor(0, 1);
    lcd.print("  Starting...  ");

    Control_Surface.begin();
    lcd.clear();
}

void loop() {
    Control_Surface.loop();

    uint32_t now = millis();
    if (now - lastUpdate >= 100) {
        lastUpdate = now;
        updateDisplay();
    }
}

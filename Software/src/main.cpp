#include <Arduino.h>
#include <Control_Surface.h>
#undef F  // avoid Arduino macro collision with MIDI_Notes::F()

// ─── MIDI Interface ───────────────────────────────────────────────────────────
USBMIDI_Interface midi;

// ─── Controls ─────────────────────────────────────────────────────────────────
CCPotentiometer knobs[] = {
    { 1, {MIDI_CC::General_Purpose_Controller_1, Channel_1} },
    { 2, {MIDI_CC::General_Purpose_Controller_2, Channel_1} },
};

NoteButton button = {
    40, {MIDI_Notes::C(4), Channel_1},
};

// ─── MIDI output indicator ────────────────────────────────────────────────────
// Lights the LED while the note is held
NoteLED led = {
    41, {MIDI_Notes::C(4), Channel_1},
};

void setup() {
    // FilteredAnalog<12, 3>::setupADC;
    Control_Surface.begin();
}

void loop() {
    Control_Surface.loop();
}

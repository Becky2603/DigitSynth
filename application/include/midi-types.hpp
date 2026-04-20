
#ifndef MidiTypes_hpp
#define MidiTypes_hpp

#include <cstdint>

/**
  * Designates the control mode for the synth logic layer.
 */
enum ControlMode {
    NORMAL,
    CHORD
};

/**
  * Designates the LED pattern for use by LedController.
 */
enum LedPattern {
    STATUS,
    RIPPLE
};

/**
  * Designates the LFO shape; used by LedController and the MIDI output layer.
 */
enum LfoShape {
    SIN,
    SQR,
    SH
};

/**
  * Stores an abstract MIDI message
 */
struct midi_message {
    uint8_t status;
    uint8_t data_1;
    uint8_t data_2;
};

#endif /* MidiTypes_hpp */

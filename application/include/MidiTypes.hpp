
#ifndef MidiTypes_hpp
#define MidiTypes_hpp

#include <cstdint>

enum ControlMode {
    NORMAL,
    CHORD
};

enum LedPattern {
    STATUS,
    RIPPLE
};

enum LfoShape {
    SIN,
    SQR,
    SH
};

struct midi_message {
    uint8_t status;
    uint8_t data_1;
    uint8_t data_2;
};

#endif /* MidiTypes_hpp */

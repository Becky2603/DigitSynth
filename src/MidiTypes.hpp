
#ifndef MidiTypes_hpp
#define MidiTypes_hpp

#include <stdio.h>

#endif /* MidiTypes_hpp */

enum ControlMode {
    EQ,
    SOURCE_EQ,
    DETUNE,
    CHORD
};

struct midi_message {
    uint8_t status;
    uint8_t data_1;
    uint8_t data_2;
};

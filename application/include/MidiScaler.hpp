#ifndef MidiScaler_hpp
#define MidiScaler_hpp

#include <stdio.h>
#include <cstdint>
#include <cassert>

class MidiScaler {
public:
    uint8_t scaleValue(float value);
};

#endif /* MidiScaler_hpp */

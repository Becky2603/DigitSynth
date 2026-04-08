#include "MidiScaler.hpp"
#include <cmath>
#include <cstdint>

uint8_t MidiScaler::scaleValue(float value){ // assuming for now that value is in range 0-1
    uint8_t scaled_value = round(value * 127);
    if (scaled_value > 127){
        scaled_value = 127;
    }
    else if (scaled_value < 0){
        scaled_value = 0;
    }
    return scaled_value;
}

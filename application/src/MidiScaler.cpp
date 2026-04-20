#include "MidiScaler.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>

uint8_t MidiScaler::scaleValue(float value){ // assuming for now that value is in range 0-1
    uint8_t scaled_value = (uint8_t) std::clamp(round(value * 127), 0.0, 127.0);
    return scaled_value;
}

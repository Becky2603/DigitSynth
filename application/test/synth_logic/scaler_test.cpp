#include <stdio.h>
#include "MidiScaler.hpp"
#include <cmath>
#include <cstdint>

int main(){
    MidiScaler scaler;
    
    uint8_t test_1 = scaler.scaleValue(0); // expected value 0
    assert(test_1 == 0);
    
    uint8_t test_2 = scaler.scaleValue(1); // expected value 127
    assert(test_2 == 127);
    
    uint8_t test_3 = scaler.scaleValue(0.65); // expected 83, rounding up
    assert(test_3 == 83);
    
    uint8_t test_4 = scaler.scaleValue(0.64); // expected 81, rounding down
    assert(test_4 == 81);
    
    uint8_t test_5 = scaler.scaleValue(-0.1); // out of bounds, expected 0
    assert(test_5 == 0);
    
    uint8_t test_6 = scaler.scaleValue(1.5); // out of bounds, expected 127
    assert(test_6 == 127);
    
    return 0;
}

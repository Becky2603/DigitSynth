#include "voltage-scaler.hpp"
#include <ads1115rpi.h>
#include <cassert>

void testChannelSegregation() {
    auto vs = voltage_scaler::VoltageScaler();
    
    for (int i = 0; i < 4; i++) {
        float offset = i * 10.0;
        vs.scale(offset, (ADS1115settings::Input) i);
        vs.scale(offset + 10.0, (ADS1115settings::Input) i);
    }
    
    for (int i = 0; i < 4; i++) {
        assert(vs.scale(i * 10.0 + 5.0, (ADS1115settings::Input) i) == 0.5);
    }
}

void testAutoRanging() {
    auto ch = ADS1115settings::AIN0;
    
    auto vs = voltage_scaler::VoltageScaler();
    vs.scale(0, ch);
    vs.scale(5, ch);
    assert(vs.scale(2.5, ch) == 0.5);
    
    vs = voltage_scaler::VoltageScaler();
    vs.scale(0, ch);
    vs.scale(-10, ch);
    assert(vs.scale(-2.5, ch) == 0.75);
    
    vs = voltage_scaler::VoltageScaler();
    vs.scale(0, ch);
    assert(vs.scale(0.1, ch) == 1); 
}

int main() {
   testAutoRanging();
   testChannelSegregation();
    
    return 0; 
}

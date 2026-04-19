#include "voltage-scaler.h"
#include <ads1115rpi.h>

bool testChannelSegregation() {
    bool success = true;
    
    auto vs = voltage_scaler::VoltageScaler();
    
    for (int i = 0; i < 4; i++) {
        float offset = i * 10.0;
        vs.scale(offset, (ADS1115settings::Input) i);
        vs.scale(offset + 10.0, (ADS1115settings::Input) i);
    }
    
    for (int i = 0; i < 4; i++) {
        success &= vs.scale(i * 10.0 + 5.0, (ADS1115settings::Input) i) == 0.5;
    }
    
    return success;
}

bool testAutoRanging() {
    auto ch = ADS1115settings::AIN0;
    bool success = true; 
    
    auto vs = voltage_scaler::VoltageScaler();
    vs.scale(0, ch);
    vs.scale(5, ch);
    success &= vs.scale(2.5, ch) == 0.5;
    
    vs = voltage_scaler::VoltageScaler();
    vs.scale(0, ch);
    vs.scale(-10, ch);
    success &= vs.scale(-2.5, ch) == 0.75;
    
    vs = voltage_scaler::VoltageScaler();
    vs.scale(0, ch);
    success &= vs.scale(0.1, ch) == 1; 
    
    return success;
}

int main() {
    bool success = true;
    
    success &= testAutoRanging();
    success &= testChannelSegregation();
    
    return success ? 0 : 1; 
}

#ifndef VOLTAGE_SCALER_H_
#define VOLTAGE_SCALER_H_

#include <ads1115rpi.h>
#include <limits>

namespace voltage_scaler {
    
class IVoltageScaler {
public:    
    virtual float scale(float v, ADS1115settings::Input channel) = 0;
}; 

class VoltageScaler : public IVoltageScaler {
public: 
    VoltageScaler();

    float scale(float v, ADS1115settings::Input channel) override;
    
private:
    std::unordered_map<ADS1115settings::Input, float> mins = {
        { ADS1115settings::AIN0, std::numeric_limits<float>::max() },
        { ADS1115settings::AIN1, std::numeric_limits<float>::max() },
        { ADS1115settings::AIN2, std::numeric_limits<float>::max() },
        { ADS1115settings::AIN3, std::numeric_limits<float>::max() },
    };
    std::unordered_map<ADS1115settings::Input, float> maxes = {
        { ADS1115settings::AIN0, 0.0 },
        { ADS1115settings::AIN1, 0.0 },
        { ADS1115settings::AIN2, 0.0 },
        { ADS1115settings::AIN3, 0.0 },
    };
};

}

#endif

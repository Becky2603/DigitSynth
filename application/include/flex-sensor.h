#ifndef FLEX_SENSOR_H_
#define FLEX_SENSOR_H_

#include <ads1115rpi.h>
#include <optional>
#include <unordered_map>

#include "adc-driver.h"
#include "types.h"

class FlexSensor {
public:
    
    FlexSensor();

    void begin();
    void registerCallback(ExtensionCallback);
    
private: 
    void updateIfNeeded();

    const float V_MAX = 5.0;
    ADS1115rpi::ADSCallbackInterface adsCallback;
    std::unordered_map<ADS1115settings::Input, ExtensionData> values = {
        { ADS1115settings::AIN0, 0.0 },
        { ADS1115settings::AIN1, 0.0 },
        { ADS1115settings::AIN2, 0.0 },
        { ADS1115settings::AIN3, 0.0 },
    };
    std::optional<ExtensionCallback> callback = {};     
    AdcDriver adc; 
    ADS1115settings::Input currentChannel = ADS1115settings::AIN0;
    
};

#endif

#ifndef FLEX_SENSOR_H_
#define FLEX_SENSOR_H_

#include <ads1115rpi.h>
#include <memory>
#include <optional>
#include <unordered_map>

#include "adc-driver.hpp"
#include "voltage-scaler.hpp"

namespace flex_sensor {
using ExtensionData = float;      
using ExtensionCallback = std::function<void(std::array<ExtensionData, 4>)>;

class IFlexSensor {
public: 
    virtual ~IFlexSensor() = default;
    
    virtual void registerCallback(ExtensionCallback) = 0;
    virtual void begin() = 0;
};
    
/**
 * When `begin()` is called, this class will sample each channel of the 
 * ADC in turn and call the registered callback once every channel has been
 * sampled. 
 */
class FlexSensor : public IFlexSensor {
public:
        
    FlexSensor(std::unique_ptr<adc_driver::IAdcDriver> adcDriver, std::unique_ptr<voltage_scaler::IVoltageScaler> voltageScaler);
    ~FlexSensor();

    /**
     * Start sampling the sensors.
     */
    void begin();
    
    /**
     * Register the function that will be called when data is ready. 
     * @param callback --- the callback to be registered. 
     */
    void registerCallback(ExtensionCallback callback);
    
    uint64_t getNSamples();
    
private: 
    /**
     * Will call the callback if all ADC channels have been sampled.
     */
    void updateIfNeeded();
    
    float mapVoltage(float f, ADS1115settings::Input channel);

    ADS1115rpi::ADSCallbackInterface adsCallback;
    std::unordered_map<ADS1115settings::Input, ExtensionData> values = {
        { ADS1115settings::AIN0, 0.0 },
        { ADS1115settings::AIN1, 0.0 },
        { ADS1115settings::AIN2, 0.0 },
        { ADS1115settings::AIN3, 0.0 },
    };

    std::optional<ExtensionCallback> callback = {};     
    std::unique_ptr<adc_driver::IAdcDriver> adc;
    std::unique_ptr<voltage_scaler::IVoltageScaler> vs; 
    
    ADS1115settings::Input currentChannel = ADS1115settings::AIN0;
    
    
    std::thread worker;
    bool running = true;
    
    uint64_t n_samples = 0;
};

}
#endif

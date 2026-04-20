#ifndef ADC_DRIVER_H_
#define ADC_DRIVER_H_

#include <ads1115rpi.h>
#include <optional>

namespace adc_driver {

using AdcData = std::optional<uint32_t>;
    
class IAdcDriver {
public: 
    virtual ~IAdcDriver() = default;
    
    virtual void readChannel(ADS1115settings::Input, ADS1115rpi::ADSCallbackInterface *) = 0;
};
    
class Ads1115Driver : public IAdcDriver {
public: 

    Ads1115Driver();
    ~Ads1115Driver(); 
    
    
    /**
     * Reads a given channel of the ADC and responds when data ready. Handles switching channels and discarding invalid data. 
     * @param channel --- channel from which to read
     * @param callback --- callback to be called when data has been read
     */
    void readChannel(ADS1115settings::Input channel, ADS1115rpi::ADSCallbackInterface *callback);

private: 
    ADS1115rpi ads; 
    ADS1115settings settings;  
    int samplesSinceChannelChange; 
};

}
#endif

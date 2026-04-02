#ifndef ADC_DRIVER_H_
#define ADC_DRIVER_H_

#include <ads1115rpi.h>
#include <optional>
#include <thread>

class AdcDriver {
public: 

    AdcDriver();
    ~AdcDriver(); 
    
    
    void readChannel(ADS1115settings::Input, ADS1115rpi::ADSCallbackInterface *);

private: 
    ADS1115rpi ads; 
    ADS1115settings settings;  
    int samplesSinceChannelChange; 
};

#endif

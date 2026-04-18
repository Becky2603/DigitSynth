
#include "flex-sensor.h"
#include <ads1115rpi.h>
#include <array>

void FlexSensor::updateIfNeeded() {
    if (this->currentChannel != ADS1115settings::AIN0 || !this->callback.has_value()) { return; } 
    
    std::array<ExtensionData, 4> data;
    data[0] = this->values[ADS1115settings::AIN0];
    data[1] = this->values[ADS1115settings::AIN1];
    data[2] = this->values[ADS1115settings::AIN2];
    data[3] = this->values[ADS1115settings::AIN3];
    
    this->n_samples++;
    this->callback.value()(data);
}

uint64_t FlexSensor::getNSamples() {
    return this->n_samples;
}

FlexSensor::FlexSensor() {
    this->adsCallback = [&] (float f) {
        this->values[this->currentChannel] =  (f / FlexSensor::V_MAX) + FlexSensor::V_OFF;
        
        switch (this->currentChannel) {
            case (ADS1115settings::AIN0):
                this->currentChannel = ADS1115settings::AIN1;
                break;
                
            case (ADS1115settings::AIN1):
                this->currentChannel = ADS1115settings::AIN2;
                break;

            case (ADS1115settings::AIN2):
                this->currentChannel = ADS1115settings::AIN3;
                break;
 
            case (ADS1115settings::AIN3):
                this->currentChannel = ADS1115settings::AIN0;
                break;               
        }
        
        this->adc.readChannel(this->currentChannel, &this->adsCallback);
        this->updateIfNeeded();
    };
}

void FlexSensor::registerCallback(ExtensionCallback callback) {
    this->callback = callback;
}

void FlexSensor::begin() {
    this->adc.readChannel(this->currentChannel, &this->adsCallback);
}

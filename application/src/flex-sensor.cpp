
#include "flex-sensor.h"
#include <ads1115rpi.h>
#include <array>
#include <thread>

using namespace flex_sensor;

void FlexSensor::updateIfNeeded() {
    if (!this->callback.has_value()) { return; } 
    
    std::array<ExtensionData, 4> data;
    data[0] = mapVoltage(this->values[ADS1115settings::AIN0], ADS1115settings::AIN0);
    data[1] = mapVoltage(this->values[ADS1115settings::AIN1], ADS1115settings::AIN1);
    data[2] = mapVoltage(this->values[ADS1115settings::AIN2], ADS1115settings::AIN2);
    data[3] = mapVoltage(this->values[ADS1115settings::AIN3], ADS1115settings::AIN3);
    
    this->n_samples++;
    this->callback.value()(data);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    
    // std::cout << std::string(1000, '\b');
    for (int i = 0; i < 4; i++) {
        auto channel = (ADS1115settings::Input) i;
        printf("| %.3f\t%.3f\t%.3f\t%.3f |", data[i], this->values[channel], this->maxes[channel], this->mins[channel]);
    }
    printf("\n");
}

uint64_t FlexSensor::getNSamples() {
    return this->n_samples;
}

float FlexSensor::mapVoltage(float f, ADS1115settings::Input channel) {
    if (f > this->maxes[channel]) { this->maxes[channel] = f; }
    if (f < this->mins[channel])  { this->mins[channel] =  f; }
    
    return (f - this->mins[channel]) / (this->maxes[channel] - this->mins[channel]);
}

FlexSensor::FlexSensor() {
    this->adsCallback = [&] (float f) {
        auto prevChannel = this->currentChannel;
        
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
                this->updateIfNeeded();
                break;               
        }
        
        this->values[prevChannel] = f;  
        this->adc.readChannel(this->currentChannel, &this->adsCallback);
    };
}

void FlexSensor::registerCallback(ExtensionCallback callback) {
    this->callback = callback;
}

void FlexSensor::begin() {
    this->adc.readChannel(this->currentChannel, &this->adsCallback);
}

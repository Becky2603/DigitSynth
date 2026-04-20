
#include "flex-sensor.h"
#include "adc-driver.h"
#include "voltage-scaler.h"
#include <ads1115rpi.h>
#include <array>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>

using namespace flex_sensor;

void FlexSensor::updateIfNeeded() {
    if (!this->callback.has_value() || this->currentChannel != ADS1115settings::AIN0) { return; } 
    
    std::array<ExtensionData, 4> data;
    data[0] = this->vs->scale(this->values[ADS1115settings::AIN0], ADS1115settings::AIN0);
    data[1] = this->vs->scale(this->values[ADS1115settings::AIN1], ADS1115settings::AIN1);
    data[2] = this->vs->scale(this->values[ADS1115settings::AIN2], ADS1115settings::AIN2);
    data[3] = this->vs->scale(this->values[ADS1115settings::AIN3], ADS1115settings::AIN3);
    
    this->n_samples++;
    // std::cout << "updating...\n";
    this->callback.value()(data);
}

uint64_t FlexSensor::getNSamples() {
    return this->n_samples;
}

FlexSensor::FlexSensor(std::unique_ptr<adc_driver::IAdcDriver> adcDriver, std::unique_ptr<voltage_scaler::IVoltageScaler> voltageScaler) 
: adc(std::move(adcDriver)), vs(std::move(voltageScaler)) 
{
    
    this->adsCallback = [&] (float f) {
        this->values[this->currentChannel] = f;  
        this->dataReady.store(true);
        
        std::unique_lock lock(this->m);
        this->c.notify_all();
        lock.unlock();
        std::cout << "cb finished\n";
    };    
    
    this->worker = std::thread([&] () { while (this->running) {
        std::unique_lock lock(this->m);
        this->c.wait(lock, [&] { return this->dataReady.load(); });
        
        switch (this->currentChannel) {
            case (ADS1115settings::Input::AIN0):
                this->currentChannel = ADS1115settings::Input::AIN1;
                break;
            case (ADS1115settings::Input::AIN1):
                this->currentChannel = ADS1115settings::Input::AIN2;
                break;               
            case (ADS1115settings::Input::AIN2):
                this->currentChannel = ADS1115settings::Input::AIN3;
                break;
            case (ADS1115settings::Input::AIN3):
                this->currentChannel = ADS1115settings::Input::AIN0;
                break;
        }
        
        this->updateIfNeeded();
        
        this->adc->readChannel(this->currentChannel, &this->adsCallback);
        lock.unlock();
    } std::cout << "LOOP DONE\n"; });
    
    this->adc->readChannel(this->currentChannel, &this->adsCallback);
}

FlexSensor::~FlexSensor() {
    this->running = false; 
    std::unique_lock lock(this->m);
    this->c.notify_all();
    lock.unlock();
    if (worker.joinable()) { worker.join(); }
}

void FlexSensor::registerCallback(ExtensionCallback callback) {
    std::cout << "callback registered\n";
    this->callback = callback;
}

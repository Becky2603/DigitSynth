#include "adc-driver.h"
#include "flex-sensor.h"
#include "voltage-scaler.h"
#include <ads1115rpi.h>
#include <chrono>
#include <cstddef>
#include <random>
#include <thread>

#define KNOWN_VALUE (42.42)

class MockAdcDriver : public adc_driver::IAdcDriver {
public: 

    int n = 0.0; 
    
    void readChannel(ADS1115settings::Input channel, ADS1115rpi::ADSCallbackInterface *callback) override {
        (void) channel; 
        
        std::random_device rd; // obtain a random number from hardware
        std::mt19937 gen(rd()); // seed the generator
        std::uniform_int_distribution<> distr(1, 1000); // define the range
    
        std::this_thread::sleep_for(std::chrono::microseconds(distr(gen)));
        
        n += 1; 
        std::cout << KNOWN_VALUE + n << std::endl;
        (*callback)(KNOWN_VALUE + n);
    }
};

class MockVoltageScaler : public voltage_scaler::IVoltageScaler {
    
    float scale(float v, ADS1115settings::Input channel) {
        (void) channel;
        
        return v; 
    }
};

void testReceiveData() {
    auto fs = new flex_sensor::FlexSensor(
        static_cast<adc_driver::IAdcDriver *>(new MockAdcDriver), 
        static_cast<voltage_scaler::IVoltageScaler *>(new MockVoltageScaler)
    );
    
    std::vector<std::array<flex_sensor::ExtensionData, 4>> samples;
    
    
    auto callback = [&samples] (std::array<flex_sensor::ExtensionData, 4> f) {
        samples.push_back(f);
    };
    fs->registerCallback(callback);
    fs->begin();
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    delete fs;
    
    for (auto &f : samples) {
        for (size_t i = 0; i < f.size(); i++) {
            for (size_t j = 0; j < f.size(); j++) {
                if (i == j) { continue; }
                if (f[i] == f[j]) { std::cout << f[i] << f[j] << "\n"; assert(false); }
            }
        }
    }
}

int main() {
    testReceiveData();
    
    return 0; 
}

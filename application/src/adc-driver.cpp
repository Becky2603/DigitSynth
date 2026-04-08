#include "adc-driver.h"
#include <ads1115rpi.h>

AdcDriver::AdcDriver() : samplesSinceChannelChange{0} {
    this->settings.samplingRate = ADS1115settings::FS860HZ;
    this->settings.drdy_gpio = 22;
    this->ads.start(this->settings);
}

AdcDriver::~AdcDriver() {
    this->ads.stop();
}

void AdcDriver::readChannel(ADS1115settings::Input channel, ADS1115rpi::ADSCallbackInterface *callback) {
    if (this->ads.getADS1115settings().channel != channel) {
        this->samplesSinceChannelChange = 0;
    } else {
        this->samplesSinceChannelChange = 2; 
    }
    
    this->ads.setChannel(channel);
    this->ads.registerCallback([&, callback] (float f) {
        this->samplesSinceChannelChange++;
        if (this->samplesSinceChannelChange >= 2) { (*callback)(f); }
    });
}

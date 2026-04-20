#include "adc-driver.h"
#include <ads1115rpi.h>
#include <thread>

using namespace adc_driver;

Ads1115Driver::Ads1115Driver() : samplesSinceChannelChange{0} {
    this->settings.samplingRate = ADS1115settings::FS860HZ;
    this->settings.drdy_gpio = 22;
    this->ads.start(this->settings);
}

Ads1115Driver::~Ads1115Driver() {
    this->ads.stop();
}

void Ads1115Driver::readChannel(ADS1115settings::Input channel, ADS1115rpi::ADSCallbackInterface *callback) {
    if (this->ads.getADS1115settings().channel != channel) {
        this->samplesSinceChannelChange = 0;
    } else {
        this->samplesSinceChannelChange = 2; 
    }
    
    this->ads.setChannel(channel);
    this->ads.registerCallback([&, callback] (float f) {
        this->samplesSinceChannelChange++;
        std::cout << this->samplesSinceChannelChange << " " << std::this_thread::get_id() << std::endl;
        if (this->samplesSinceChannelChange >= 2) { (*callback)(f); }
    });
}

#include "adc-driver.h"
#include "types.h"

AdcDriver::AdcDriver() {}


void AdcDriver::notifySubscribers(AdcData data) {
    for (auto &callback : this->callbacks) {
        callback(data);
    }
}

void AdcDriver::registerCallback(AdcCallback callback) {
    this->callbacks.push_back(callback);
}

void AdcDriver::beginContinuous() {

    this->continuousConversion = std::thread([] {

    });
}



void AdcDriver::cancel() {
    this->conversionActive = false;
}

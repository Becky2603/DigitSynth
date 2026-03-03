#include "adc-driver.h"
#include "callback-types.h"

AdcDriver::AdcDriver() {}


void AdcDriver::notifySubscribers(AdcData data) {
    for (auto &callback : this->callbacks) {
        callback(data);
    }
}

void AdcDriver::registerCallback(AdcCallback callback) {
    this->callbacks.push_back(callback);
}

void AdcDriver::begin() {
    this->active = true;

    while (this->active) {
        // wait on data-ready

        // collect data

        AdcData data = 0;
        notifySubscribers(data);
    }
}

void AdcDriver::cancel() {
    this->active = false;
}

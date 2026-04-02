#include "adc-driver.h"
#include "callback-types.h"

#include <thread>

/* 
bool test_singleSubscriber() {
    AdcDriver adcDriver;
    AdcData x = 10;
    adcDriver.registerCallback(std::function<void(AdcData)>([&x, &adcDriver] (AdcData data) { x = data; adcDriver.cancel(); }));

    std::thread t = std::thread([&adcDriver] {
        adcDriver.beginContinuous();
    });

    t.join();

    return x == 0 ? 0 : 1;
}

bool test_multipleSubscribers() {
    AdcDriver adcDriver;
    int x = 0;

    for (int i = 0; i < 10; i++) {
        adcDriver.registerCallback(std::function<void(AdcData)>([&x, &adcDriver] (AdcData data) {
            if (data.has_value()) {
                x += 1;
            }

            if (x == 10) { adcDriver.cancel(); }
        }));
    }

    std::thread t = std::thread([&adcDriver] {
        adcDriver.beginContinuous();
    });

    t.join();

    return x == 10 ? 0 : 1;
}
*/

int main() {
    bool success = true;

    // success &= test_singleSubscriber();
    // success &= test_multipleSubscribers();

    return success ? 0 : 1;
}

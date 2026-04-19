#include "button-driver.h"
#include "gpio.h"

using namespace button_driver; 

bool test_registerButtonCallback() {
    ButtonDriver bd; 
    
    bd.registerButtonCallback([] (ButtonIndex idx) {
        (void) idx; 
    });
    
    return true;
}

/**
 * Make sure that destroying an instance of `ButtonDriver` frees up the GPIO pins.
 * If the destruction does not work properly then the `::setPin` calls should cause
 * an exception. 
 */
bool test_destruct() {
    auto bd = new ButtonDriver();
    
    delete bd; 

    gpio::setPin(14, true);
    gpio::setPin(15, true);
    gpio::setPin(18, true);
    gpio::setPin(23, true);

    
    return true;
}

int main() {
    gpio::setupGpio();
    
    bool success = true;
    
    success &= test_registerButtonCallback();
    success &= test_destruct();
    
    
    return success ? 0 : -1;
}

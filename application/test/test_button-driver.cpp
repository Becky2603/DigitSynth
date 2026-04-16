#include "button-driver.h"
#include "gpio.h"
#include "types.h"

bool test_registerSingleButtonCallback() {
    ButtonDriver bd; 
    
    bd.registerSingleButtonCallback([] (ButtonIndex idx) {
        (void) idx; 
    });
    
    return true;
}

bool test_registerAllButtonsCallback() {
    ButtonDriver bd; 
    
    bd.registerAllButtonsCallback([] (void) {
    
    });
    
    return true;
}

bool test_singleButton() {
    ButtonDriver bd; 
    bool booped = false;
   
    bd.registerSingleButtonCallback([&booped] (ButtonIndex idx) {
        booped = idx == 0;
    });
    
    gpio::setPin(18, false);
    gpio::setPin(18, true);
    gpio::setPin(18, false);
    
    return booped;
}

int main() {
    gpio::setupGpio();
    
    bool success = true;
    
    success &= test_registerSingleButtonCallback();
    success &= test_registerAllButtonsCallback();
    
    
    return success ? 0 : -1;
}

#include "button-driver.h"
#include "gpio.h"
#include "types.h"


int main() {
    
    gpio::setupGpio();
    ButtonDriver bd;
    
    bd.registerSingleButtonCallback([] (ButtonIndex index) {
        std::cout << index;
    });
    
    getchar();
    
    gpio::teardownGpio();
    
    return 0;
}

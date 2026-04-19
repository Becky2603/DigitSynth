
#include "SynthController.hpp"
#include "TLC59711.h"
#include "button-driver.h"
#include "gpio.h"
#include <iostream>

int main() {
    gpio::setupGpio();
    
    auto bd = new button_driver::ButtonDriver(); 
    TLC59711 tlc(17, 27);
    tlc.start();
    
    // 
    // std::array<Brightness, 8> leds = {1, 1, 1, 1, 1, 1, 1, 1,};
    // tlc.update(leds);
    // getchar();
    // 
    // for (int i = 0; i < 8; i++) {
        // std::cout << i << std::endl;
        // for (int j = 0; j < 8; j++) {
            // if (i==j) {leds[j] = 1.0;}
            // else {leds[j] = 0.0;}
        // }
        // tlc.update(leds);
        // 
        // getchar();
    // }
    
    tlc.start();
    SynthController synth(tlc, static_cast<button_driver::IButtonDriver *>(bd));
    bd = nullptr; 
    getchar();
    std::cout <<"stop\n";
    
    
    
    return 0;
}

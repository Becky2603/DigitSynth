
#include "SynthController.hpp"
#include "TLC59711.h"
#include "button-driver.h"
#include "flex-sensor.h"
#include "gpio.h"
#include <iostream>

int main() {
    gpio::setupGpio();
    
    auto bd = new button_driver::ButtonDriver(); 
    auto fs = new flex_sensor::FlexSensor();
    auto tlc = new led_driver::TLC59711(17, 27);
    
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
    
    tlc->start();
    SynthController synth(static_cast<led_driver::ILedDriver *>(tlc), static_cast<button_driver::IButtonDriver *>(bd), static_cast<flex_sensor::IFlexSensor *>(fs));
    bd = nullptr;
    fs = nullptr; 
    tlc = nullptr;
    getchar();
    std::cout <<"stop\n";
    
    
    
    return 0;
}

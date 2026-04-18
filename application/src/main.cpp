
#include "ITLC59711.hpp"
#include "MidiTypes.hpp"
#include "SynthController.hpp"
#include "TLC59711.h"
#include "button-driver.h"
#include "flex-sensor.h"
#include "gpio.h"
#include "midi-driver.hpp"
#include "patterns.h"
#include "types.h"
#include <iostream>

int main() {
    gpio::setupGpio();
    
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
    SynthController synth(tlc);
    getchar();
    std::cout <<"stop\n";
    
    
    
    return 0;
}

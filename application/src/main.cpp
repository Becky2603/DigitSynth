
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
    SynthController synth(tlc);
    getchar();
    std::cout <<"stop\n";
    
    
    
    return 0;
}

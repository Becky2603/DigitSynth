#include <chrono>
#include <cmath>
#include <cstring>
#include <gpiod.hpp>
#include <iostream>
#include <thread>
#include "adc-driver.h"
#include "foo.h"
#include "gpio.h"
#include "spi.h"


int main(int argc, char **argv) {
    (void) argc;
    (void) argv; 
    
    gpio::setupGpio();
    
    gpio::registerCallback(22, gpiod::line::edge::RISING, [] () {
        gpio::setPin(26, 1);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        gpio::setPin(26, 0);
    });
    while (1) {
    }
    
    return 0;
}

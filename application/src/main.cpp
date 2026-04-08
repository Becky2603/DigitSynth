#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <gpiod.hpp>
#include <iostream>
#include "button-driver.h"
#include "flex-sensor.h"
#include "foo.h"
#include "gpio.h"
#include "types.h"
#include <linux/spi/spi.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <ads1115rpi.h>

void callback(float f) {
    std::cout << f << std::endl;
    fflush(stdout);
} 

/**
 * @returns woohoo
 */
int main(int argc, char **argv) {
    (void) argc;
    (void) argv; 
  
    gpio::setupGpio();
    
    ButtonDriver bd {};
    
    bd.registerSingleButtonCallback([] (uint32_t idx) {
        std::cout << "single button pressed: " << idx << std::endl;
    });
    
    bd.registerAllButtonsCallback([] () {
        std::cout << "all buttons pressed\n";
    });
    
    getchar();
    
    
    return 0;
}

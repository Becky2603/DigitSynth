#include <cmath>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <gpiod.hpp>
#include <iostream>
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

int main(int argc, char **argv) {
    (void) argc;
    (void) argv; 
  
    gpio::setupGpio();
    FlexSensor sensor;
    
    sensor.begin();
    sensor.registerCallback([] (std::array<ExtensionData, 4> data) {
        std::cout << data[0] << std::endl;
        std::cout << data[1] << std::endl;
        std::cout << data[2] << std::endl;
        std::cout << data[3] << std::endl;
        std::cout << std::endl;
    });
   
    getchar();
    
    (void) sensor;
    
    return 0;
}

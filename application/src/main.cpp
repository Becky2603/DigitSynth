#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <gpiod.hpp>
#include <iostream>
#include "adc-driver.h"
#include "foo.h"
#include "gpio.h"
#include <linux/spi/spi.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <thread>
#include <unistd.h>
#include <ads1115rpi.h>

void callback(float f) {
    std::cout << f << std::endl;
    fflush(stdout);
} 

int main(int argc, char **argv) {
    (void) argc;
    (void) argv; 
    
     
    /* 
    int fd = open("/dev/spidev0.0", O_RDWR);
    
    uint8_t mode = SPI_MODE_1;
    uint8_t bitsPerWord = 8;
    // uint8_t bitOrder = settings.bitOrder;

    std::cout << "setting mode\n";
    checkError_l(ioctl(fd, SPI_IOC_RD_MODE,          &mode));
    std::cout << "set mode\n";
    checkError_l(ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bitsPerWord));
    std::cout << "set bits per word\n";
    int speed = 1;
    checkError_l(ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ,  &speed));
    std::cout << "set clock\n";

    uint8_t data[1] = { 0b01010011 };
    ::write(fd, data, 1);
    */
   
    gpio::setupGpio();
   
    auto ads = ADS1115rpi();
    ads.registerCallback(callback);
    
    auto settings = ADS1115settings();
    
    ads.start(settings);
    
    getchar();
    
    return 0;
}

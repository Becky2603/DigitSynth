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
    
    SpiSettings spiSettings;
    spiSettings.bitsPerWord = 8;
    spiSettings.clockFrequency = 2000000;
    Spi spi("/dev/spidev0.0", spiSettings); 
    
    AdcSettings adcSettings;
    adcSettings.analogueInputBuffer = false;
    adcSettings.autoCalibration = false;
    adcSettings.clockOut = false;
    adcSettings.clockRate = AdsClockRate::R1000;
    adcSettings.logGain = 1;
    adcSettings.lsbFirst = true;
    AdcDriver adc(&spi, adcSettings);
    
    
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    int period = 1000000000 / 7680000;
    
    gpio::setPin(11, 0);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    gpio::setPin(11, 1);
    std::this_thread::sleep_for(std::chrono::nanoseconds(400 * period));
    gpio::setPin(11, 0);
    std::this_thread::sleep_for(std::chrono::nanoseconds(10 * period));
    gpio::setPin(11, 1);
    std::this_thread::sleep_for(std::chrono::nanoseconds(650 * period));
    gpio::setPin(11, 0);
    std::this_thread::sleep_for(std::chrono::nanoseconds(10 * period));
    gpio::setPin(11, 1);
    std::this_thread::sleep_for(std::chrono::nanoseconds(1150 * period));
    gpio::setPin(11, 0);
    
    adc.writeCommand(Ads1256Command::STANDBY);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    
    adc.writeCommand(Ads1256Command::WAKEUP);
    std::this_thread::sleep_for(std::chrono::microseconds(100));
    adc.writeCommand(Ads1256Command::RDATA);
    
    std::vector<uint8_t> vec(3);
    spi.read(vec);
    adc.writeCommand(Ads1256Command::STANDBY);
    uint32_t val = 0;
    memcpy(&val, vec.data(), vec.size());
    std::cout << val << std::endl;
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    return 0; 
    
    adc.readChannel(0, [] (AdcData data) {
        if (data.has_value()) { std::cout << "value: " << data.value() << std::endl;}
        else { std::cout << "no data\n"; }
    });
    
    
    return 0;
}

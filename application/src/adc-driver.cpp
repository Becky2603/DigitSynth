#include "adc-driver.h"
#include "pin-map.h"
#include "types.h"

#include <array>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <vector>
#include <wiringPi.h>

#define DRDY    GPIO15

AdcDriver::AdcDriver(Spi *spi, AdcSettings settings) : spi(spi) {
    auto device = this->spi->addDevice();
    if (!device.has_value()) { std::cerr << "Not enough SPI devices to instantiate ADC\n"; exit(-1); }
    
    this->spiDevice = device.value();
    this->clockPeriod_ms = 1000000 / adsClockToFrequency(settings.clockRate);
    
    // The AND 0x01 here is to ensure that higher-level bits that may have been accidentally set are not included.
    uint8_t statusVal = 0x00, ioVal = 0x00;
    statusVal |= (settings.lsbFirst & 0x01) << 3;
    statusVal |= (settings.autoCalibration & 0x01) << 2; 
    statusVal |= (settings.analogueInputBuffer & 0x01) << 1;
    
    uint8_t muxVal = 0x11; // +'ve channel 0 selected, -'ve AINCOM selected
    
    uint8_t adconVal = (settings.logGain & 0b00000111); 
    
    this->writeRegister(statusVal, Ads1256Register::STATUS);
    this->writeRegister(muxVal,    Ads1256Register::MUX);
    this->writeRegister(adconVal,  Ads1256Register::ADCON);
    this->writeRegister(ioVal,     Ads1256Register::IO);
    
    this->writeCommand(SELFCAL);
    
    pinMode(DRDY, INPUT);
}

void AdcDriver::writeRegister(uint8_t value, Ads1256Register reg) {
    std::array<uint8_t, 4> buf;
    buf[0] = 0b01010000 | (0x0f & reg); 
    buf[1] = 0x00; // writing 1 byte
    buf[2] = 0x00;
    buf[3] = value;
    
    std::vector<uint8_t> vec(buf.begin(), buf.end()); 
    ::write(this->spi->fd, buf.data(), buf.size());
    
    if (reg == Ads1256Register::MUX) {
        writeCommand(Ads1256Command::SYNC);
        writeCommand(Ads1256Command::WAKEUP);
    }
}

void AdcDriver::writeCommand(Ads1256Command command) {
    std::vector<uint8_t> buf; buf.push_back(command); 
    ::write(this->spi->fd, buf.data(), buf.size());
    // this->spi.get()->write(buf, this->spiDevice);
}

void AdcDriver::readChannel(AdcChannel channel, AdcCallback callback) {
    channel &= 0b00000111; // this will have the side-effect of an invalid channel becoming 0
    
    uint8_t muxVal = (channel << 4) | 0x08;
    this->writeRegister(muxVal, Ads1256Register::MUX);
    
    while (digitalRead(DRDY)) { waitForInterrupt2(DRDY, INT_EDGE_FALLING, -1, 0); }
    
    this->writeCommand(RDATA);
    std::this_thread::sleep_for(std::chrono::microseconds(50 * this->clockPeriod_ms));
    
    std::vector<uint8_t> vec(3);

    int br = ::read(spi.get()->fd, vec.data(), vec.size());
    std::cerr << br << std::endl;
    (void) br; 
    uint32_t val = 0;
    memcpy(&val, vec.data(), vec.size());
    // val >>= 8; 

    callback(val);
    
    /*
    this->spi.get()->read(&vec, this->spiDevice, [vec, callback] (ssize_t bytesRead) {
        std::cout << "bytes read: " << bytesRead << std::endl;
        std::cout << "vec len: " << vec.size() << std::endl;
        if (bytesRead != 16) { callback({}); return; }
        
        uint32_t val = 22;
        memcpy(&val, vec.data(), vec.size());
        val >>= 8; 
    
        callback(val);
        });*/
}

uint32_t AdcDriver::adsClockToFrequency(AdsClockRate rate) {
    switch (rate) {
        case AdsClockRate::R30000: return 30000;
        case AdsClockRate::R15000: return 15000;
        case AdsClockRate::R7500:  return 7500;
        case AdsClockRate::R3750:  return 3750;
        case AdsClockRate::R2000:  return 2000;
        case AdsClockRate::R1000:  return 1000;
        case AdsClockRate::R500:   return 500;
        default: return 100;
    }
}

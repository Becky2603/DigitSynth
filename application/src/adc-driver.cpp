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
#include <vector>
#include <wiringPi.h>

#define DRDY    GPIO15

AdcDriver::AdcDriver(std::shared_ptr<Spi> spi, AdcSettings settings) : spi(spi) {
    auto device = this->spi->addDevice();
    if (!device.has_value()) { std::cerr << "Not enough SPI devices to instantiate ADC\n"; exit(-1); }
    
    this->spiDevice = device.value();
    this->clockPeriod_ms = 1000000 / settings.clockRate;
    
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
    
    pinMode(DRDY, INPUT);
}

void AdcDriver::writeRegister(uint8_t value, Ads1256Register reg) {
    std::array<uint8_t, 4> buf;
    buf[0] = 0b0101000 | (0x0f & reg); 
    buf[1] = 0x00; // writing 1 byte
    buf[2] = 0x00;
    buf[3] = value;
    
    std::vector<uint8_t> vec(buf.begin(), buf.end()); 
    this->spi.get()->write(vec, this->spiDevice);
}

void AdcDriver::writeCommand(Ads1256Command command) {
    std::vector<uint8_t> buf; buf.push_back(command); 
    this->spi.get()->write(buf, this->spiDevice);
}

void AdcDriver::readChannel(AdcChannel channel, AdcCallback callback) {
    channel &= 0b00000111; // this will have the side-effect of an invalid channel becoming 0
    
    uint8_t muxVal = channel << 4;
    this->writeRegister(muxVal, Ads1256Register::MUX);
    
    if (digitalRead(DRDY)) { waitForInterrupt2(DRDY, INT_EDGE_FALLING, -1, 0); }
    
    this->writeCommand(RDATA);
    std::this_thread::sleep_for(std::chrono::microseconds(50 * this->clockPeriod_ms));
    
    std::array<uint8_t, 3> buf{{0, 0, 0}}; 
    std::vector<uint8_t> vec(buf.begin(), buf.end());
    
    this->spi.get()->read(&vec, this->spiDevice, [buf, callback] (ssize_t bytesRead) {
        if (bytesRead != 3) { callback({}); return; }
        
        uint32_t val;
        memcpy(&val, buf.data(), buf.size());
        val >>= 8; 
    
        callback(val);
    });
}

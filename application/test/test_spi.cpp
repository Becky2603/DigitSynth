#include "spi.h"
#include <cstdint>
#include <iostream>


bool test_canWrite() {
    
    SpiSettings settings{};
    settings.bitOrder = 0;
    settings.bitsPerWord = 8;
    settings.clockFrequency = 1000;
    settings.clockPhase = 1;
    settings.clockPolarity = 0;
    
    std::cout << settings.bitOrder; 
    
    Spi spi("/dev/spidev0.0", settings);
    
    // auto device = spi.addDevice().value();
    // std::vector<uint8_t> buf(24);
    // spi.write(buf, device);
    
    return true;  
}

int main() {
    bool success = true; 
    
    success &= test_canWrite();
    
    return success ? 0 : -1;
}

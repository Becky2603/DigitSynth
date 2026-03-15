#ifndef ADC_DRIVER_H_
#define ADC_DRIVER_H_

#include <cstdint>
#include <memory>
#include <vector>
#include <thread>

#include <stddef.h>
#include <wiringPi.h>

#include "spi.h"
#include "types.h"

enum Ads1256Register {
    STATUS = (uint8_t) 0x00,
    MUX,
    ADCON,
    DRATE,
    IO,
    OFC0,
    OFC1,
    OFC2, 
    FSC0, 
    FSC1, 
    FSC2,
};

enum Ads1256Command {
    WAKEUP   = 0x00,    // n.b. wakeup can also be 0xff
    RDATA    = 0x01,
    RDATAC   = 0x03,
    SDATAC   = 0x0f,
    RREG     = 0x10,    // n.b. OR with register number 0000 rrrr
    WREG     = 0x50,    // n.b. OR with register number 0000 rrrr
    SELFCAL  = 0xf0,
    SELFOCAL = 0xf1,
    SELFGCAL = 0xf2,
    SYSOCAL  = 0xf3,
    SYSGCAL  = 0xf4,
    SYNC     = 0xfc,
    STANDBY  = 0xfd,
    RESET    = 0xfe,
};

enum AdsClockRate {
    R30000 = (uint8_t) 0b11110000,
    R15000           = 0b11100000,
    R7500            = 0b11010000,
    R3750            = 0b11000000,
    R2000            = 0b10110000,
    R1000            = 0b10100001,
    R500             = 0b10010010,
    R100             = 0b10000010,
    R60              = 0b01110010,
    R50              = 0b01100011,
    R30              = 0b01010011,
    R25              = 0b01000011,
    R15              = 0b00110011,
    R10              = 0b00100011,
    R5               = 0b00010011,
    R2_5             = 0b00000011, 
};

struct AdcSettings {
    bool    lsbFirst: 1,
            autoCalibration: 1,
            analogueInputBuffer: 1,
            clockOut: 1;
            
    uint8_t logGain: 3; 

    AdsClockRate clockRate;    
            
};

class AdcDriver {
public:
    AdcDriver(std::shared_ptr<Spi>, AdcSettings);
    void readChannel(AdcChannel, AdcCallback);

private:
    std::shared_ptr<Spi> spi;
    SpiDevice spiDevice; 
    uint32_t clockPeriod_ms;
    void writeRegister(uint8_t, Ads1256Register); 
    void writeCommand(Ads1256Command);
    
};

#endif

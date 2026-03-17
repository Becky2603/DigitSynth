#include <cstring>
#include <iostream>
#include <wiringPi.h>
#include "adc-driver.h"
#include "foo.h"
#include "spi.h"


int main(int argc, char **argv) {
    std::cout << "\nStarting DigitSynth application\nArguments: \n";
    
    wiringPiSetupGpio();

    for (int i = 0; i < argc; i++) {
    	std::cout << "\t" << argv[i] << "\n"; 
    }

    SpiSettings spiSettings{};
    spiSettings.bitOrder = 0;
    spiSettings.bitsPerWord = 8;
    spiSettings.clockFrequency = 1'000'000;
    spiSettings.clockPolarity = 0;
    spiSettings.clockPhase = 1; 
    Spi spi("/dev/spidev0.0", spiSettings);
    
    AdcSettings adcSettings{};
    adcSettings.analogueInputBuffer = false;
    adcSettings.autoCalibration = false;
    adcSettings.clockOut = false;
    adcSettings.clockRate = AdsClockRate::R1000;
    adcSettings.logGain = 0;
    adcSettings.lsbFirst = true;
    AdcDriver adc(&spi, adcSettings);
    
    adc.readChannel(0, [] (AdcData data) {
        std::cout << strerror(errno) << std::endl;
        std::cout << data.value() << std::endl;
    });
    
    
    return 0;
}

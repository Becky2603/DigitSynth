#ifndef SPI_H_
#define SPI_H_

#include <condition_variable>
#include <deque>
#include <memory>
#include <thread>
#include <optional>
#include <string>
#include <vector>
#include <cstdint>

#include "types.h"

#define SPI_MAX_DEVICES 4

typedef uint8_t SpiDevice;

struct SpiSettings {
    uint8_t   bitsPerWord:   8,
              clockPolarity: 1, // Polarity of the clock during idle phase
              clockPhase:    1, // Data read/written on rising edge (0) or falling edge (1)
              bitOrder:      1; // LSB first (1) or MSB first (0)

    Frequency clockFrequency;
};

/**
 * Each instance of this class represents one SPI interface, not one SPI device. 
 * Multiple devices can be added to this class, each using a different ~CS pin. 
 */
class Spi {
public:

    void write(std::vector<uint8_t>, SpiDevice);
    void read(std::vector<uint8_t> *, SpiDevice, SpiCallback);

    void updateSettings(SpiSettings);
    
    /**
     * @return none if there are already SPI_MAX_DEVICES devices in use, an SpiDevice otherwise 
     */
    std::optional<SpiDevice> addDevice();
    
    Spi(std::string, SpiSettings);
    ~Spi();

private:
    int fd;
    uint8_t nDevices;  
    std::string path;
    std::deque<std::tuple<std::unique_ptr<std::vector<uint8_t>>, SpiDevice, SpiCallback>> readQueue; 
    std::deque<std::tuple<std::vector<uint8_t>, SpiDevice>> writeQueue; 
    std::thread worker; 
    std::mutex mut;
    std::condition_variable cond;
    
    // TODO: decide on pins
    static const constexpr uint8_t csPins[SPI_MAX_DEVICES] = {
        18,
        19,
        20, 
        21, 
    };

};

#endif

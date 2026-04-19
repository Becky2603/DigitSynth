#ifndef ILedDriver_hpp
#define ILedDriver_hpp

#include "types.h"
#include <array>

/* Led driver interface for dependency inversion: anything that wants to be an
 LED driver must be able to update() channels
 
 TLC59711 is the real LED driver
 MockTLC59711 just records what is sent to it and is used for unit testing the LedController class
*/

namespace led_driver {

class ILedDriver {
public:
    static constexpr int NUM_LEDS = 8;
    using Channels = std::array<Brightness, NUM_LEDS>;
    
    virtual ~ILedDriver() = default;
    virtual void update(const Channels& channels) = 0;
};

}

#endif /* ILedDriver_hpp */

#ifndef ITLC59711_hpp
#define ITLC59711_hpp

#include "types.h"
#include <array>

/* TLC interface for dependency injection: anything that wants to be an
 LED driver must be able to update() channels
 
 TLC59711 is the real LED driver
 MockTLC59711 just records what is sent to it and is used for unit testing the LedController class
*/

class ITLC59711 {
public:
    static constexpr int NUM_LEDS = 8;
    using Channels = std::array<Brightness, NUM_LEDS>;

    virtual ~ITLC59711() = default;
    virtual void update(const Channels& channels) = 0;
};

#endif /* ITLC59711_hpp */

#ifndef MockTLC59711_hpp
#define MockTLC59711_hpp

#include "ILedDriver.hpp"

namespace led_driver {

class MockTLC59711 : public ILedDriver { //a fake TLC that records update() calls for unit testing LedController class
public:
    void update(const Channels& channels) override { // records channels and updates a flag instead of driving real hardware
        lastChannels = channels;
        updateCalled = true;
    }
    Channels lastChannels{};
    bool updateCalled = false;
};

}

#endif /* MockTLC59711_hpp */

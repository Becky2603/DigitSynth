#ifndef MockTLC59711_hpp
#define MockTLC59711_hpp

#include "i-led-driver.hpp"

namespace led_driver {

/**
 * Test double for led_driver::ILedDriver.
 *
 * Records the most recent channel values passed to update(), allowing unit tests
 * to assert on LED output without requiring real hardware.
 * Used in LedController unit tests.
 */
class MockTLC59711 : public ILedDriver {
public:
    /**
     * Records the channel values and sets updateCalled to true.
     * Does not drive any real hardware.
     *
     * @param channels Array of brightness values to record
     */
    void update(const Channels& channels) override {
        lastChannels = channels;
        updateCalled = true;
    }

    Channels lastChannels{}; ///< Most recent channels passed to update()
    bool updateCalled = false; ///< True if update() has been called at least once
};

}

#endif /* MockTLC59711_hpp */

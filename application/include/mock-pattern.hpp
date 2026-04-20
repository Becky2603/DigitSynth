#ifndef MockPattern_hpp
#define MockPattern_hpp

#include "i-led-driver.hpp"
#include "patterns.hpp"

/**
 * Test double for led_pattern::IPattern.
 *
 * Records whether start() and stop() have been called, allowing unit tests
 * to assert on ripple pattern behaviour without running a real animation thread.
 */
class MockPattern : public led_pattern::IPattern {
public:
    MockPattern(led_driver::ILedDriver &ledDriver) : IPattern(ledDriver) {}

    /**
     * Records that start() was called. Does not start a thread.
     * @param onDone Ignored in mock implementation.
     */
    void start(led_pattern::DoneCallback onDone = nullptr) {
        (void) onDone;
        startCalled = true;
    }

    /**
     * Records that stop() was called. Does not join any thread.
     */
    void stop() { stopCalled = true; }

    void run() override {}

    bool startCalled = false; ///< True if start() has been called
    bool stopCalled  = false; ///< True if stop() has been called
};

#endif /* MockPattern_hpp */

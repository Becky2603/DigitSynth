#ifndef MockPattern_hpp
#define MockPattern_hpp

#include "ILedDriver.hpp"
#include "patterns.h"

class MockPattern : public led_pattern::IPattern {
public:
    MockPattern(led_driver::ILedDriver &ledDriver) : IPattern(ledDriver) {}

    void start(led_pattern::DoneCallback onDone = nullptr) override {
        (void) onDone; 
	    startCalled = true;
    }
    void stop() override { stopCalled = true; }
    void run() override {}
    bool startCalled = false;
    bool stopCalled = false;
};

#endif /* MockPattern_hpp */

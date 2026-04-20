#ifndef MockPattern_hpp
#define MockPattern_hpp

#include "patterns.h"

class MockPattern : public led_pattern::ILEDPattern {
public:
    void start(DoneCallback onDone = nullptr) override {
        (void)onDone;
        startCalled = true;
    }
    void stop() override { stopCalled = true; }
    bool startCalled = false;
    bool stopCalled = false;
};

#endif /* MockPattern_hpp */

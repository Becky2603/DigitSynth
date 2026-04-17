#ifndef MockPattern_hpp
#define MockPattern_hpp

#include "patterns.h"

class MockPattern : public Pattern {
public:
    void start(DoneCallback onDone = nullptr) override { startCalled = true; }
    void stop() override { stopCalled = true; }
    bool startCalled = false;
    bool stopCalled = false;
protected:
    void run() override {}
};

#endif /* MockPattern_hpp */

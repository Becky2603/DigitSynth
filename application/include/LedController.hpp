#ifndef LedController_hpp
#define LedController_hpp

#include "MidiTypes.hpp"
#include "ITLC59711.hpp"
#include "patterns.h"
#include <array>

class LedController {
public:
    explicit LedController(ITLC59711& tlc, Pattern& ripple);
    void update(ControlMode mode, bool lfoEnabled, LfoShape shape, std::array<float, 4> flexValues);
private:
    void startRipple();
    void stopRipple();
    
    ITLC59711& _tlc;
    Pattern& _ripple; //in tests we pass a MockPattern, in practice we pass a PatternRipple
    bool rippleRunning = false;
};

#endif /* LedController_hpp */

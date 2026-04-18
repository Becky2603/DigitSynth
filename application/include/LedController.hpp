#ifndef LedController_hpp
#define LedController_hpp

#include "MidiTypes.hpp"
#include "TLC59711.h"
#include "patterns.h"
#include <array>

class LedController {
public:
    explicit LedController(TLC59711& tlc, Pattern& ripple);
    void update(ControlMode mode, bool lfoEnabled, LfoShape shape, std::array<float, 4> flexValues);
private:
    void startRipple();
    void stopRipple();
    
    TLC59711& _tlc;
    Pattern& _ripple; //in tests we pass a MockPattern, in practice we pass a PatternRipple
    bool rippleRunning = false;
};

#endif /* LedController_hpp */

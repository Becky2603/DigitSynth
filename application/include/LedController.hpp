#ifndef LedController_hpp
#define LedController_hpp

#include "MidiTypes.hpp"
#include "TLC59711.h"
#include "patterns.h"
#include <array>


namespace Led {

    static constexpr int L_thumb   = 0;  ///< Thumb flex brightness
    static constexpr int L_index   = 1;  ///< Index finger flex brightness
    static constexpr int L_middle  = 2;  ///< Middle finger flex brightness
    static constexpr int L_ring    = 3;  ///< Ring finger flex brightness


    static constexpr int R_thumb   = 4;  ///< Thumb flex brightness
    static constexpr int R_index   = 5;  ///< Index finger flex brightness
    static constexpr int R_middle  = 6;  ///< Middle finger flex brightness
    static constexpr int R_ring    = 7;  ///< Ring finger flex brightness

}

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

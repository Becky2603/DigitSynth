#ifndef LedController_hpp
#define LedController_hpp

#include "MidiTypes.hpp"
#include "ILedDriver.hpp"
#include "patterns.h"
#include <unordered_map>
#include <array>

/**
 * Physical-to-logical LED channel mapping.
 *
 * The TLC59711 drives 8 channels (indices 0–7, up to G0).
 * Reassign these constants to remap any LED to a different physical channel
 * without touching any other code.
 *
 * Left hand: channels 0–3, Right hand: channels 4–7.
 * All LEDs are flex-sensor driven (brightness 0.0–1.0).
 */
namespace Led {
    // --- Left hand ---
    static constexpr int L_pinky   = 5;  ///< Thumb flex brightness
    static constexpr int L_index   = 1;  ///< Index finger flex brightness
    static constexpr int L_middle  = 4;  ///< Middle finger flex brightness
    static constexpr int L_ring    = 0;  ///< Ring finger flex brightness

    // --- Right hand ---
    static constexpr int R_pinky   = 7;  ///< Thumb flex brightness
    static constexpr int R_index   = 2;  ///< Index finger flex brightness
    static constexpr int R_middle  = 6;  ///< Middle finger flex brightness
    static constexpr int R_ring    = 3;  ///< Ring finger flex brightness
}

class LedController {
public:
    explicit LedController(led_driver::ILedDriver& tlc, Pattern& ripple, std::unordered_map<LfoShape, float> shapeBrightness);
    void update(ControlMode mode, bool lfoEnabled, LfoShape shape, std::array<float, 4> flexValues);
    void togglePattern();
private:
    void startRipple();
    void stopRipple();
    
    led_driver::ILedDriver& _tlc;
    Pattern& _ripple; //in tests we pass a MockPattern, in practice we pass a PatternRipple
    
    const std::unordered_map<LfoShape, float> shapeBrightness;
    
    bool rippleRunning = false;
    
    LedPattern pattern = STATUS;
};

#endif /* LedController_hpp */

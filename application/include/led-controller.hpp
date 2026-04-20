#ifndef LedController_hpp
#define LedController_hpp

#include "midi-types.hpp"
#include "i-led-driver.hpp"
#include "patterns.hpp"
#include <unordered_map>
#include <array>

/**
 * @brief Physical-to-logical LED channel mapping.
 *
 * The TLC59711 provides 12 output channels (indices RGB0–4).
 * These constants define the logical mapping of finger LEDs to physical channels for the 8 used LEDs.
 *
 * changing these values allows remapping LEDs without modifying application logic.
 *
 * @note Channel assignments do not strictly follow left/right grouping;
 *       they reflect the physical wiring layout.
 *
 */
namespace Led {
    static constexpr int L_pinky   = 5; 
    static constexpr int L_index   = 1;  
    static constexpr int L_middle  = 4; 
    static constexpr int L_ring    = 0;  


    static constexpr int R_pinky   = 7;  
    static constexpr int R_index   = 2;  
    static constexpr int R_middle  = 6;  
    static constexpr int R_ring    = 3;  
}
/**
 * @class LedController
 * @brief Controls LED behavior based on control modes, LFO settings, and patterns.
 *
 * The LedController coordinates between a LED driver and a pattern generator
 * to produce visual effects. It supports dynamic updates based on control input,
 * including LFO-driven modulation and pattern switching (e.g., ripple effects).
 *
 * Dependencies are injected via interfaces, allowing for flexible implementations
 * and easy unit testing (e.g., using mock patterns).
 */
class LedController {
public:
    /**
     * @brief Constructs a LedController instance.
     *
     * @param tlc Reference to an LED driver implementation responsible for
     *        low-level LED control.
     * @param pattern Reference to a pattern implementation that defines
     *        LED animation behavior.
     * @param shapeBrightness Mapping of LFO shapes to brightness scaling factors.
     */
    explicit LedController(led_driver::ILedDriver& tlc,
                           led_pattern::IPattern& pattern,
                           std::unordered_map<LfoShape, float> shapeBrightness);

    /**
     * @brief Updates the LED state based on current control inputs.
     *
     * This method should be called periodically to refresh LED output.
     * It adjusts brightness, pattern behavior, and optional LFO modulation.
     *
     * @param mode Current control mode determining LED behavior.
     * @param lfoEnabled Whether LFO modulation is active.
     * @param shape The selected LFO waveform shape.
     * @param flexValues Array of sensor/input values influencing LED output.
     */
    void update(ControlMode mode,
                bool lfoEnabled,
                LfoShape shape,
                std::array<float, 4> flexValues);

    /**
     * @brief Toggles the active LED pattern.
     *
     * Switches between available patterns.
     */
    void togglePattern();

private:
    /**
     * @brief Starts the ripple LED pattern.
     *
     * Activates a ripple animation if not already running.
     */
    void startRipple();

    /**
     * @brief Stops the ripple LED pattern.
     *
     * Deactivates the ripple animation and returns to the default pattern.
     */
    void stopRipple();

    led_driver::ILedDriver& _tlc;       
    led_pattern::IPattern& _pattern;     

    /**
     * @brief Mapping of LFO shapes to brightness scaling factors.
     *
     * Used to adjust LED intensity depending on the selected waveform.
     */
    const std::unordered_map<LfoShape, float> shapeBrightness;

    bool rippleRunning = false; 

    LedPattern _ledPattern = STATUS; 
};

#endif /* LedController_hpp */

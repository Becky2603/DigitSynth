#ifndef LfoManager_hpp
#define LfoManager_hpp

#include "midi-types.hpp"
#include <vector>

/**
 * Manages LFO state for the DigitSynth.
 *
 * Tracks whether the LFO is enabled and which shape is currently active.
 * Shapes are stored as an ordered list, allowing new shapes to be added
 * without modifying any cycling logic — obeying the Open/Closed Principle.
 *
 * LFO state is read by SynthController to determine which MIDI messages
 * to send and by LedController to drive the appropriate LED indicators.
 */
class LfoManager {
public:
    /**
     * Toggles the LFO between enabled and disabled states.
     */
    void toggle();

    /**
     * Advances to the next LFO shape in the cycle, wrapping back to the
     * first shape after the last.
     */
    void cycleShape();

    /**
     * Returns whether the LFO is currently enabled.
     *
     * @return true if enabled, false if disabled
     */
    bool isEnabled() const;

    /**
     * Returns the currently active LFO shape.
     *
     * @return Current LfoShape value
     */
    LfoShape getShape() const;

private:
    bool enabled = true;

    /// Ordered list of available LFO shapes. Add new shapes here to extend
    /// without modifying any other logic.
    const std::vector<LfoShape> shapes = {SIN, SQR, SH};
    int shapeIndex = 0;
};

#endif

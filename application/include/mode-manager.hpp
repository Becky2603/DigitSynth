#ifndef ModeManager_hpp
#define ModeManager_hpp

#include "midi-types.hpp"

/**
 * Tracks the current control mode of DigitSynth.
 *
 * The instrument operates in one of two modes:
 *  - NORMAL: flex sensors and buttons control sound parameters (cutoff, LFO, pitch bend)
 *  - CHORD:  buttons select chords; pressing the active chord button exits chord mode
 *
 * Defaults to NORMAL on construction.
 */
class ModeManager {
public:
    /**
     * Returns the current control mode.
     *
     * @return Current ControlMode (NORMAL or CHORD)
     */
    ControlMode getCurrentMode();

    /**
     * Toggles between NORMAL and CHORD modes.
     *
     */
    void updateMode();

private:
    ControlMode current_controlMode = NORMAL;
};

#endif /* ModeManager_hpp */

#ifndef SynthController_hpp
#define SynthController_hpp

#include "ModeManager.hpp"
#include "paramMapper.hpp"
#include "MidiScaler.hpp"
#include "ChordManager.hpp"
#include "TLC59711.h"
#include "patterns.h"
#include <cstdint>
#include <memory>
#include <optional>

class SynthController {
public:
    // TLC59711 is passed in by reference — SynthController uses it but does
    // not own it. The caller (main) owns the hardware and its lifetime.
    explicit SynthController(TLC59711& tlc);

    void onButtonEvent(int index);
    void onFlexEvent(int index, float value);
    void onAllButtonsPressed();

    ControlMode getCurrentMode(); // for testing
    uint8_t getCurrentChord();   // for testing
    midi_message getLastCC();    // for testing

private:
    void startRipple();
    void stopRipple();
    void startFade();
    void stopFade();
    
    TLC59711&                    _tlc;
    PatternRipple _ripple;  // null when not in chord mode
    PatternFade _fade;

    ModeManager  modeManager;
    ChordManager chordManager;
    ParamMapper  paramMapper;
    MidiScaler   midiScaler;

    midi_message lastCC;
};

#endif /* SynthController_hpp */

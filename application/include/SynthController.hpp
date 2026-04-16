#ifndef SynthController_hpp
#define SynthController_hpp

#include "ModeManager.hpp"
#include "button-driver.h"
#include "paramMapper.hpp"
#include "MidiScaler.hpp"
#include "ChordManager.hpp"
#include <cstdint>
#include <memory>
#include "types.h"
#include "TLC59711.h"
#include "patterns.h"
#include "flex-sensor.h"
#include "FlexDSP.h"

class SynthController {
public:
    // TLC59711 is passed in by reference — SynthController uses it but does
    // not own it. The caller (main) owns the hardware and its lifetime.
    explicit SynthController(TLC59711& tlc);
    ~SynthController();

    ControlMode getCurrentMode(); // for testing
    uint8_t getCurrentChord();   // for testing
    midi_message getLastCC(int i);    // for testing
    
    void registerMidiCallback(MidiCallback callback);

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
    
    std::optional<MidiCallback> midiCallback = {};

    midi_message lastCC[4];
    
    ButtonDriver buttonDriver;
    FlexDSP flexDSP;
};

#endif /* SynthController_hpp */

#ifndef SynthController_hpp
#define SynthController_hpp

#include "ModeManager.hpp"
#include "button-driver.h"
#include "MessageBuilder.hpp"
#include "MidiScaler.hpp"
#include "ChordManager.hpp"
#include <cstdint>
#include <memory>
#include "types.h"
#include "TLC59711.h"
#include "patterns.h"
#include "flex-sensor.h"

class SynthController {
public:
    // TLC59711 is passed in by reference — SynthController uses it but does
    // not own it. The caller (main) owns the hardware and its lifetime.
    explicit SynthController(TLC59711& tlc);
    ~SynthController();
    
    void registerMidiCallback(MidiCallback callback);

private:
    void startRipple();
    void stopRipple();
    
    TLC59711&                    _tlc;
    PatternRipple _ripple;  // null when not in chord mode

    ModeManager  modeManager;
    ChordManager chordManager;
    MessageBuilder messageBuilder;
    MidiScaler   midiScaler;
    LfoManager lfoManager;
    
    std::optional<MidiCallback> midiCallback = {};
    
    ButtonDriver buttonDriver;
    FlexSensor   flexSensor;
};

#endif /* SynthController_hpp */

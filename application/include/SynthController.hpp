#ifndef SynthController_hpp
#define SynthController_hpp

#include "ModeManager.hpp"
#include "button-driver.h"
#include "MessageBuilder.hpp"
#include "MidiScaler.hpp"
#include "ChordManager.hpp"
#include "LedController.hpp"
#include <cstdint>
#include <optional>
#include "midi-driver.hpp"
#include "types.h"
#include "ITLC59711.hpp"
#include "patterns.h"
#include "LfoManager.hpp"
#include "FlexDSP.hpp"

class SynthController {
public:
    //TLC59711 passed in by reference — SynthController uses it but doesn't own it
    explicit SynthController(TLC59711& tlc);
    ~SynthController();

private:
    ModeManager  modeManager;
    ChordManager chordManager;
    MessageBuilder messageBuilder;
    MidiScaler   midiScaler;
    LfoManager lfoManager;
    MidiDriver midiDriver; 
    
    PatternRipple _ripple;
    
    LedController ledController;
    
    ButtonDriver buttonDriver;
    FlexDSP flexDSP;
};

#endif /* SynthController_hpp */

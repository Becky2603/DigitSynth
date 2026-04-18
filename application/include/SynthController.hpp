#ifndef SynthController_hpp
#define SynthController_hpp

#include "ModeManager.hpp"
#include "button-driver.h"
#include "MessageBuilder.hpp"
#include "MidiScaler.hpp"
#include "ChordManager.hpp"
#include "LedController.hpp"
#include "midi-driver.hpp"
#include "types.h"
#include "TLC59711.h"
#include "patterns.h"
#include "LfoManager.hpp"
#include "FlexDSP.hpp"

class SynthController {
public:
    //TLC59711 passed in by reference — SynthController uses it but doesn't own it
    explicit SynthController(TLC59711& tlc, button_driver::IButtonDriver *buttonDriver);
    ~SynthController();
    
    void registerMidiCallback(MidiCallback callback);
private:
    ModeManager  modeManager;
    ChordManager chordManager;
    MessageBuilder messageBuilder;
    MidiScaler   midiScaler;
    LfoManager lfoManager;
    MidiDriver midiDriver; 
    
    PatternRipple _ripple;
    
    LedController ledController;
    
    std::unique_ptr<button_driver::IButtonDriver> buttonDriver;
    FlexDSP flexDSP;
};

#endif /* SynthController_hpp */

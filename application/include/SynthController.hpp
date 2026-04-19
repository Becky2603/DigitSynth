#ifndef SynthController_hpp
#define SynthController_hpp

#include "ModeManager.hpp"
#include "button-driver.h"
#include "MessageBuilder.hpp"
#include "MidiScaler.hpp"
#include "ChordManager.hpp"
#include "LedController.hpp"
#include "flex-sensor.h"
#include "midi-driver.hpp"
#include "patterns.h"
#include "LfoManager.hpp"
#include "FlexDSP.hpp"
#include <memory>

class SynthController {
public:
    //TLC59711 passed in by reference — SynthController uses it but doesn't own it
    explicit SynthController(
        TLC59711& tlc, 
        button_driver::IButtonDriver *buttonDriver, 
        flex_sensor::IFlexSensor     *flexSensor, 
        midi_driver::IMidiDriver     *midiDriver
    );
    ~SynthController();

private:
    ModeManager  modeManager;
    ChordManager chordManager;
    MessageBuilder messageBuilder;
    MidiScaler   midiScaler;
    LfoManager lfoManager;
    std::unique_ptr<midi_driver::IMidiDriver> midiDriver; 
    
    PatternRipple _ripple;
    
    LedController ledController;
    
    std::unique_ptr<button_driver::IButtonDriver> buttonDriver;
    FlexDSP flexDSP;
};

#endif /* SynthController_hpp */

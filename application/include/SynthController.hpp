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
#include "types.h"
#include "TLC59711.h"
#include "patterns.h"
#include "flex-sensor.h"
#include "LfoManager.hpp"

class SynthController {
public:
    //TLC59711 passed in by reference — SynthController uses it but doesn't own it
    explicit SynthController(TLC59711& tlc);
    ~SynthController();
    
    void registerMidiCallback(MidiCallback callback);
private:
    ModeManager  modeManager;
    ChordManager chordManager;
    MessageBuilder messageBuilder;
    MidiScaler   midiScaler;
    LfoManager lfoManager;
    LedController ledController;
    
    std::optional<MidiCallback> midiCallback = {};
    
    ButtonDriver buttonDriver;
    FlexSensor   flexSensor;
};

#endif /* SynthController_hpp */

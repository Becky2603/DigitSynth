#ifndef SynthController_hpp
#define SynthController_hpp

#include "ModeManager.hpp"
#include "paramMapper.hpp"
#include "MidiScaler.hpp"
#include "ChordManager.hpp"
#include <cstdint>

class SynthController {
public:
    void onButtonEvent(int index); //callback for a single button press
    void onFlexEvent(int index, float value); //callback for a flex sensor update
    void onAllButtonsPressed(); //callback for every button pressed at same time
    
    ControlMode getCurrentMode(); // for testing
    uint8_t getCurrentChord(); // for testing
    midi_message getLastCC(); // for testing
private:
    ModeManager modeManager;
    ChordManager chordManager;
    ParamMapper paramMapper;
    MidiScaler midiScaler;
    
    midi_message lastCC; // for testing
};

#endif /* SynthController_hpp */

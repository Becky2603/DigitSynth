#include "SynthController.hpp"
#include <iostream>

SynthController::SynthController(TLC59711& tlc)
: _ripple(tlc), ledController(tlc, _ripple)
{
    this->buttonDriver.registerSingleButtonCallback([this] (int index) {
        if (modeManager.getCurrentMode() == NORMAL){
            switch(index){
                case 0: // enter chord mode
                    modeManager.updateMode();
                    break;
                case 1: // enable/disable the LFO
                    lfoManager.toggle();
                    break;
                case 2: // cycle through the LFO shapes
                    lfoManager.cycleShape();
                    break;
                case 3: // turn off/on
                    
                    break;
            }
        }
        else { // chord mode
            if (index == chordManager.getCurrentChord()){
                modeManager.updateMode(); // exit chord mode, back to normal
            }
            else {
                chordManager.updateChord(index);
            }
        }
    });
    
    this->flexSensor.registerCallback([this] (std::array<ExtensionData, 4> values){
        if (modeManager.getCurrentMode() == NORMAL){
            for (int i = 0; i < 4; i++){
                uint8_t scaledVal = midiScaler.scaleValue(values[i]);
                if (i == 2 && !lfoManager.isEnabled()) {
                    scaledVal = 64; // middle value
                }
                auto messages = messageBuilder.buildMessages(i, scaledVal);
                for (auto& msg : messages){
                    if (midiCallback.has_value()){
                        midiCallback.value()(msg);
                    }
                }
                
            }
        }
        else {
            for (int i = 0; i < 4; i++){
                uint8_t velocity = midiScaler.scaleValue(values[i]);
                uint8_t note = chordManager.getNote(i);
                midi_message msg = {0x90, note, velocity};
                if (midiCallback.has_value()){
                    midiCallback.value()(msg);
                }
            }
        }
        ledController.update(modeManager.getCurrentMode(), lfoManager.isEnabled(), lfoManager.getShape(), {values[0], values[1], values[2], values[3]});
    });
}

void SynthController::registerMidiCallback(MidiCallback callback){
    midiCallback = callback;
}

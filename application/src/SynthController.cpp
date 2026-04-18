#include "SynthController.hpp"
#include "FlexDSP.hpp"
#include "TLC59711.h"
#include "button-driver.h"
#include "flex-sensor.h"
#include <memory>

SynthController::SynthController(TLC59711& tlc, button_driver::IButtonDriver *buttonDriver)
: _ripple(tlc), ledController(tlc, _ripple), buttonDriver(buttonDriver)
{
    
    auto ports = this->midiDriver.listOutputPorts();

    std::cout << "Available MIDI output ports:\n";
    for (size_t i = 0; i < ports.size(); ++i) {
        std::cout << i << ": " << ports[i] << '\n';
    }

    if (ports.empty()) {
        std::cerr << "No MIDI output ports found.\n";
        exit(-1);
    }

    this->midiDriver.openPort(2);
    
    this->buttonDriver.get()->registerButtonCallback([this] (int index) {
        std::cout << "\nbutton pressed " << index << std::endl;
        if (modeManager.getCurrentMode() == NORMAL){
            midi_message msg; 
            switch(index){
                case 0: // enter chord mode
                    modeManager.updateMode();
                    break;
                case 1: // enable/disable the LFO
                    lfoManager.toggle();
                    break;
                case 2: // cycle through the LFO shapes
                    lfoManager.cycleShape();
                    msg = {0xB0, 3, static_cast<uint8_t>(lfoManager.getShape())};
                    midiDriver.sendMessage(msg);
                    break;
                default:
                    break;
            }
        }
        else { // chord mode
            if (index == chordManager.getCurrentChord()){
                modeManager.updateMode(); // exit chord mode, back to normal
            }
            else {
                // send note offs for current chord
                for (int i = 0; i < 4; i++){
                    midi_message noteOff = {0x80, chordManager.getNote(i), 0};
                    midiDriver.sendMessage(noteOff);
                }
                chordManager.updateChord(index);
            }
        }
    });
    
    this->flexDSP.registerCallback([this] (std::array<ExtensionData, 4> values){
        if (modeManager.getCurrentMode() == NORMAL){
            for (int i = 0; i < 4; i++){
                uint8_t scaledVal = midiScaler.scaleValue(values[i]);
                if (i == 2 && !lfoManager.isEnabled()) {
                    scaledVal = 64; // middle value
                }
                auto messages = messageBuilder.buildMessages(i, scaledVal);
                for (auto& msg : messages){
                    midiDriver.sendMessage(msg);
                }
                
            }
        }
        else {
            for (int i = 0; i < 4; i++){
                uint8_t velocity = midiScaler.scaleValue(values[i]);
                uint8_t note = chordManager.getNote(i);
                midi_message msg = {0x90, note, velocity};
                midiDriver.sendMessage(msg);
            }
        }
        ledController.update(modeManager.getCurrentMode(), lfoManager.isEnabled(), lfoManager.getShape(), {values[0], values[1], values[2], values[3]});
    });
    
}

SynthController::~SynthController() {
}

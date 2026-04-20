#include "SynthController.hpp"
#include "FlexDSP.hpp"
#include "ILedDriver.hpp"
#include "button-driver.h"
#include "flex-sensor.h"
#include "midi-driver.hpp"
#include "patterns.h"
#include <memory>
#include <thread>
#include <chrono>

SynthController::SynthController(
    led_driver::ILedDriver &tlc,
    Pattern &pattern,
    std::unique_ptr<button_driver::IButtonDriver> buttonDriver,
    std::unique_ptr<flex_sensor::IFlexSensor> flexSensor, 
    std::unique_ptr<midi_driver::IMidiDriver> midiDriver
): 
    midiDriver(std::move(midiDriver)), 
    ledController(tlc, pattern, {
        {SIN, 0.0f},
        {SQR, 0.5f},
        {SH,  1.0f}       
    }), 
    buttonDriver(std::move(buttonDriver)), 
    flexDSP(std::move(flexSensor))
{
    
    auto ports = this->midiDriver->listOutputPorts();

    std::cout << "Available MIDI output ports:\n";
    for (size_t i = 0; i < ports.size(); ++i) {
        std::cout << i << ": " << ports[i] << '\n';
    }

    if (ports.empty()) {
        std::cerr << "No MIDI output ports found.\n";
        exit(-1);
    }

    this->midiDriver->openPort(2);
    
    //give the synth a moment to initialise
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    
    //play Am11 on boot
    for (int i = 0; i < 6; i++){
        midi_message msg = {0x90, this->chordManager.getNote(i), 120};
        this->midiDriver->sendMessage(msg);
    }
    
    this->buttonDriver->registerSingleButtonCallback([this] (button_driver::ButtonIndex index) {
        std::cout << "\nbutton pressed " << index << std::endl;
        if (this->modeManager.getCurrentMode() == NORMAL){
            midi_message msg; 
            switch(index){
                case 0: // enter chord mode
                    this->modeManager.updateMode();
                    break;
                case 1: // enable/disable the LFO
                    this->lfoManager.toggle();
                    break;
                case 2: // cycle through the LFO shapes
                    this->lfoManager.cycleShape();
                    msg = {0xB0, 3, static_cast<uint8_t>(this->lfoManager.getShape())};
                    this->midiDriver->sendMessage(msg);
                    break;
                case 3: // change LED pattern
                    this->ledController.togglePattern();
                    break;
                default:
                    break;
            }
        }
        else { // chord mode
            if (index == this->chordManager.getCurrentChord()){
                this->modeManager.updateMode(); // exit chord mode, back to normal
            }
            else {
                // send note offs for current chord
                for (int i = 0; i < 6; i++){
                    midi_message noteOff = {0x80, this->chordManager.getNote(i), 0};
                    this->midiDriver->sendMessage(noteOff);
                }
                this->chordManager.updateChord(index);
                for (int i = 0; i < 6; i++){
                    uint8_t note = this->chordManager.getNote(i);
                    std::cout << "sending note-on\n";
                    midi_message msg = {0x90, note, 120};
                    this->midiDriver->sendMessage(msg);
                }
            }
        }
    });
    
    this->flexDSP.registerCallback([this] (std::array<ExtensionData, 4> values){
        if (true){
            for (int i = 0; i < 4; i++){
                uint8_t scaledVal = this->midiScaler.scaleValue(values[i]);
                if (i == 2 && !this->lfoManager.isEnabled()) {
                    scaledVal = 64; // middle value
                }
                auto messages = messageBuilder.buildMessages(i, scaledVal);
                for (auto& msg : messages){
                    this->midiDriver->sendMessage(msg);
                }
                
            }
        }
        ledController.update(this->modeManager.getCurrentMode(), this->lfoManager.isEnabled(), this->lfoManager.getShape(), {values[0], values[1], values[2], values[3]});
    });
}

SynthController::~SynthController() {
    for (int i = 0; i < 6; i++){
        midi_message noteOff = {0x80, this->chordManager.getNote(i), 0};
        this->midiDriver->sendMessage(noteOff);
    }   
}

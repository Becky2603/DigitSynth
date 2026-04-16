#include "SynthController.hpp"
#include "TLC59711.h"

SynthController::SynthController(TLC59711& tlc)
    : _tlc(tlc), _ripple(tlc), _fade(_tlc)
{
    this->buttonDriver.registerSingleButtonCallback([this] (int index) {
        std::cout << "on button event called " << index << std::endl;
        // if (modeManager.getCurrentMode() == CHORD){
        if (false){
            int previousChord = chordManager.getCurrentChord();
            chordManager.updateChord(index);
            int currentChord = chordManager.getCurrentChord();
            if (currentChord != previousChord){ // i.e. we are switching to a new chord
                for (int i = 0; i < 4; i++){
                    midi_message msg;
                    msg.status = 0x90; // Note On message
                    msg.data_1 = chordManager.getNote(i);
                    msg.data_2 = 127;
                    if (midiCallback.has_value()) {
                        midiCallback.value()(msg);
                    }
                }
            }
            else{ //same chord -> do nothing
                return;
            }
        }
        else {
            ControlMode previousPattern = modeManager.getActiveLedPattern();
            modeManager.updateMode(index);
            ControlMode newMode = modeManager.getCurrentMode();

            switch (newMode) {
                case EQ:
                    if (previousPattern == SOURCE_EQ) stopFade();
                    else if (previousPattern == DETUNE) stopRipple();
                    break;

                case SOURCE_EQ:
                    if (previousPattern == DETUNE) stopRipple();
                    startFade();
                    break;

                case DETUNE:
                    if (previousPattern == SOURCE_EQ) stopFade();
                    startRipple();
                    break;

                case CHORD:
                    break;
            }
        }
    });
    
    this->buttonDriver.registerAllButtonsCallback([this] () {
            ControlMode currentMode = modeManager.getCurrentMode();
            if (currentMode != CHORD){ // under normal operation, do nothing
                return;
            }
            else { // in chord mode: exit chord mode and return to what we were doing before
                currentMode = modeManager.getPreviousMode();
                int mode_index;
                switch(currentMode){
                    case EQ:
                        mode_index = 0;
                        break;
                    case SOURCE_EQ:
                        mode_index = 1;
                        break;
                    case DETUNE:
                        mode_index = 2;
                        break;
                    case CHORD:
                        mode_index = 3;
                        break;
                }
                modeManager.updateMode(mode_index);
            }
        }
    );
    
    this->flexDSP.registerCallback([this] (std::array<ExtensionData, 4> values){
        /*
        for (auto d : values) {
            std::cout << d << " ";
        }
        std::cout << std::endl;
        
        TLC59711::Channels c{};
        for (int i = 0; i < 4; i++) {
            c[i] = values[i];
        }
        this->_tlc.update(c);
        */
        
        ControlMode currentMode = modeManager.getCurrentMode();
        ControlMode effectiveMode = (currentMode == CHORD)
            ? modeManager.getActiveLedPattern()
            : currentMode;

        if (effectiveMode == EQ) {
            TLC59711::Channels c{};   // fix the typo here too
            c[0] = 1;
            c[9] = 1;
            for (int i = 0; i < 4; i++) {
                c[i+1] = values[i];
                c[8-i] = values[i];
            }
            this->_tlc.update(c);
        }
        /*
        for (int i = 0; i < 4; i++){
            uint8_t scaled_value = midiScaler.scaleValue(values[i]);
            uint8_t cc_num;
            if (currentMode == CHORD){
                cc_num = paramMapper.getCC(i, prevMode);
            }
            else {
                cc_num = paramMapper.getCC(i, currentMode);
            }
            midi_message msg;
            msg.status = 0xB0; // Control Change
            msg.data_1 = cc_num;
            msg.data_2 = scaled_value;
            lastCC[i] = msg; // for testing
            if (midiCallback.has_value()) {
                midiCallback.value()(msg);
            }
        }
        */
    });

    this->flexSensor.begin();
}

SynthController::~SynthController() {
    std::cout << this->flexSensor.getNSamples() << std::endl;
}

ControlMode SynthController::getCurrentMode(){
    return modeManager.getCurrentMode();
}

uint8_t SynthController::getCurrentChord(){
    return chordManager.getCurrentChord();
}

midi_message SynthController::getLastCC(int i){
    return lastCC[i];
}

void SynthController::startRipple() {
    std::cout << "rippling\n";
    // PatternRipple runs in its own thread (course Ch. 3) and calls
    // tlc.update() internally on each frame — SynthController just
    // starts and stops it as mode changes dictate.
    _ripple.start();
}

void SynthController::stopRipple() {
    _ripple.stop();
}

void SynthController::startFade() {
    std::cout << "fade\n";
    _fade.start();
}

void SynthController::stopFade() {
    _fade.stop();
}

void SynthController::registerMidiCallback(MidiCallback callback){
    midiCallback = callback;
}

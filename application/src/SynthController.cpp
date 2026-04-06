#include "SynthController.hpp"

void SynthController::onButtonEvent(int index){
    if (modeManager.getCurrentMode() == CHORD){
        int previousChord = chordManager.getCurrentChord();
        chordManager.updateChord(index);
        int currentChord = chordManager.getCurrentChord();
        if (currentChord != previousChord){ // i.e. we are switching to a new chord
            for (int i = 0; i < 4; i++){
                midi_message msg;
                msg.status = 0x90; // Note On message
                msg.data_1 = chordManager.getNote(i);
                msg.data_2 = 127;
                //midiDriver.noteOnCallback(msg);
            }
        }
        else{ //same chord -> do nothing
            return;
        }
    }
    else {
        modeManager.updateMode(index);
    }
}

void SynthController::onFlexEvent(int index, float value){
    uint8_t scaled_value = midiScaler.scaleValue(value);
    ControlMode currentMode = modeManager.getCurrentMode();
    ControlMode prevMode = modeManager.getPreviousMode();
    uint8_t cc_number;
    if (currentMode == CHORD){
        cc_number = paramMapper.getCC(index, prevMode);
    }
    else {
        cc_number = paramMapper.getCC(index, currentMode);
    }
    midi_message msg;
    msg.status = 0xB0; // Control Change
    msg.data_1 = cc_number;
    msg.data_2 = scaled_value;
    //midiDriver.ccCalback(msg);
}

void SynthController::onAllButtonsPressed(){
    
}

ControlMode SynthController::getCurrentMode(){
    return modeManager.getCurrentMode();
}

uint8_t SynthController::getCurrentChord(){
    return chordManager.getCurrentChord();
}

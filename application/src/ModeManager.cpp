#include "ModeManager.hpp"

ControlMode ModeManager::getCurrentMode(){
    return current_controlMode;
}

void ModeManager::updateMode(){ // just needs to toggle between normal and chord modes
    if (current_controlMode == NORMAL){
        current_controlMode = CHORD;
    }
    else {
        current_controlMode = NORMAL;
    }
}

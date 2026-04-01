#include "ModeManager.hpp"

ControlMode ModeManager::getCurrentMode(){
    return current_controlMode;
}

ControlMode ModeManager::getPreviousMode(){
    return previous_controlMode;
}

void ModeManager::updateMode(int index){
    switch (index){
        case 0:
            current_controlMode = EQ;
            break;
        case 1:
            current_controlMode = SOURCE_EQ;
            break;
        case 2:
            current_controlMode = DETUNE;
            break;
        case 3:
            previous_controlMode = current_controlMode;
            current_controlMode = CHORD;
            break;
    }
}

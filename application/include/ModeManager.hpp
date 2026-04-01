#ifndef ModeManager_hpp
#define ModeManager_hpp

#include <stdio.h>
#include "MidiTypes.hpp"

class ModeManager {
public:
    ControlMode getCurrentMode();
    ControlMode getPreviousMode();
    void updateMode(int index);
private:
    ControlMode current_controlMode = EQ;
    ControlMode previous_controlMode = EQ;
};

#endif /* ModeManager_hpp */

#ifndef ModeManager_hpp
#define ModeManager_hpp

#include "MidiTypes.hpp"

class ModeManager {
public:
    ControlMode getCurrentMode();
    void updateMode();
private:
    ControlMode current_controlMode = NORMAL;
};

#endif /* ModeManager_hpp */

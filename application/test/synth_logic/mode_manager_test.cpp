#include <stdio.h>
#include "mode-manager.hpp"
#include <cassert>

int main(){
    ModeManager mode_manager; //create the manager
    
    //has it initialised correctly?
    assert(mode_manager.getCurrentMode() == NORMAL); //default value is normal
    
    mode_manager.updateMode();
    assert(mode_manager.getCurrentMode() == CHORD);
    
    mode_manager.updateMode();
    assert(mode_manager.getCurrentMode() == NORMAL);
    
    return 0;
}

#include <stdio.h>
#include "ModeManager.hpp"
#include <cassert>

int main(){
    ModeManager mode_manager; //create the manager
    
    //has it initialised correctly?
    assert(mode_manager.getCurrentMode() == EQ); //default value is EQ
    assert(mode_manager.getPreviousMode() == EQ); //default value
    
    //simulate button 2 press
    mode_manager.updateMode(1);
    assert(mode_manager.getCurrentMode() == SOURCE_EQ); // switched to source EQ
    assert(mode_manager.getPreviousMode() == EQ); //only updates when entering chord mode
    
    //simulate button 0 press
    mode_manager.updateMode(0);
    assert(mode_manager.getCurrentMode() == EQ); //switched to detune
    assert(mode_manager.getPreviousMode() == EQ); //only updates when entering chord mode
    
    //simulate button 3 press
    mode_manager.updateMode(2);
    assert(mode_manager.getCurrentMode() == DETUNE); //switched to detune
    assert(mode_manager.getPreviousMode() == EQ); //only updates when entering chord mode
    
    //simulate button 4 press
    mode_manager.updateMode(3);
    assert(mode_manager.getCurrentMode() == CHORD); //switched to detune
    assert(mode_manager.getPreviousMode() == DETUNE); //has now updated
    
    return 0;
}

#include "SynthController.hpp"
#include <cassert>

int main(){
    SynthController controller;
    assert(controller.getCurrentMode() == EQ); //has it initialised correctly?
    
    //initially in EQ mode
    //Test 1: switch to detune mode:
    controller.onButtonEvent(2);
    assert(controller.getCurrentMode() == DETUNE);
    
    //Test 2: switch to chord mode:
    controller.onButtonEvent(3);
    assert(controller.getCurrentMode() == CHORD);
    
    //now in chord mode
    //play chord index 1
    controller.onButtonEvent(1);
    assert(controller.getCurrentMode() == CHORD);
    assert(controller.getCurrentChord() == 1);
    
    //play chord index 0
    controller.onButtonEvent(0);
    assert(controller.getCurrentMode() == CHORD);
    assert(controller.getCurrentChord() == 0);
    
    return 0;
}

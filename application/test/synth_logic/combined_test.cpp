#include "SynthController.hpp"
#include "MidiTypes.hpp"
#include <cassert>

int main(){
    SynthController controller;
    assert(controller.getCurrentMode() == EQ);
    
    //initially in EQ mode
    //test 1: CC 20, midi value 60;
    controller.onFlexEvent(1, 0.472);
    midi_message midi = controller.getLastCC();
    assert(midi.status == 0xB0);
    assert(midi.data_1 == 20);
    assert(midi.data_2 == 60);
    
    //switch to detune mode
    controller.onButtonEvent(2);
    assert(controller.getCurrentMode() == DETUNE);
    //CC 27, midi value 127 (clamped from out of bounds)
    controller.onFlexEvent(0, 1.03);
    midi = controller.getLastCC();
    assert(midi.status == 0xB0);
    assert(midi.data_1 == 27);
    assert(midi.data_2 == 127);
    //all buttons pressed: should do nothing
    controller.onAllButtonsPressed();
    assert(controller.getCurrentMode() == DETUNE);
    
    //switch to chord mode
    controller.onButtonEvent(3);
    assert(controller.getCurrentMode() == CHORD);
    //flex sensors should still operate in detune mode
    //CC 28, midi value 25
    controller.onFlexEvent(1, 0.197);
    midi = controller.getLastCC();
    assert(midi.status == 0xB0);
    assert(midi.data_1 == 28);
    assert(midi.data_2 == 25);
    
    //exit chord mode, return to detune
    controller.onAllButtonsPressed();
    assert(controller.getCurrentMode() == DETUNE);
    //CC 30, midi value 0 (clamped from out of bounds)
    controller.onFlexEvent(3, -0.1);
    midi = controller.getLastCC();
    assert(midi.status == 0xB0);
    assert(midi.data_1 == 30);
    assert(midi.data_2 == 0);
    
    return(0);
}

#include "SynthController.hpp"
#include "MidiTypes.hpp"
#include <cassert>

int main(){
    TLC59711 tlc(27, 17);
    SynthController controller(tlc);
    assert(controller.getCurrentMode() == EQ);
    
    //initially in EQ mode
    /* Test 1:
     Flex 1: CC19, value 40 (rounded up)
     Flex 2: CC20, value 127 (clamped)
     Flex 3: CC21, value 0 (clamped)
     Flex 4: CC22, value 85 (rounded down)
     */
    std::array<ExtensionData, 4> flex_data = {0.314, 1.03, -0.1, 0.67};
    controller.onFlexEvent(flex_data);
    midi_message midi[4];
    for (int i = 0; i < 4; i++){
        midi[i] = controller.getLastCC(i);
    }
    assert(midi[0].status == 0xB0);
    assert(midi[0].data_1 == 19);
    assert(midi[0].data_2 == 40);
    assert(midi[1].status == 0xB0);
    assert(midi[1].data_1 == 20);
    assert(midi[1].data_2 == 127);
    assert(midi[2].status == 0xB0);
    assert(midi[2].data_1 == 21);
    assert(midi[2].data_2 == 0);
    assert(midi[3].status == 0xB0);
    assert(midi[3].data_1 == 22);
    assert(midi[3].data_2 == 85);
    
    //switch to detune mode
    controller.onButtonEvent(2);
    assert(controller.getCurrentMode() == DETUNE);
    /* Test 2:
     Flex 1: CC27, value 70
     Flex 2: CC28, value 80 (rounded up)
     Flex 3: CC29, value 90
     Flex 4: CC30, value 100
     */
    flex_data = {0.551, 0.62599, 0.7086, 0.787};
    controller.onFlexEvent(flex_data);
    for (int i = 0; i < 4; i++){
        midi[i] = controller.getLastCC(i);
    }
    assert(midi[0].status == 0xB0);
    assert(midi[0].data_1 == 27);
    assert(midi[0].data_2 == 70);
    assert(midi[1].status == 0xB0);
    assert(midi[1].data_1 == 28);
    assert(midi[1].data_2 == 80);
    assert(midi[2].status == 0xB0);
    assert(midi[2].data_1 == 29);
    assert(midi[2].data_2 == 90);
    assert(midi[3].status == 0xB0);
    assert(midi[3].data_1 == 30);
    assert(midi[3].data_2 == 100);
    
    //switch to chord mode
    controller.onButtonEvent(3);
    assert(controller.getCurrentMode() == CHORD);
    //flex sensors should still operate in detune mode
    /* Test 3:
     Flex 1: CC27, value 0
     Flex 2: CC28, value 127
     Flex 3: CC29, value 0
     Flex 4: CC30, value 127
     */
    flex_data = {0, 1, 0.002, 0.997};
    controller.onFlexEvent(flex_data);
    for (int i = 0; i < 4; i++){
        midi[i] = controller.getLastCC(i);
    }
    assert(midi[0].status == 0xB0);
    assert(midi[0].data_1 == 27);
    assert(midi[0].data_2 == 0);
    assert(midi[1].status == 0xB0);
    assert(midi[1].data_1 == 28);
    assert(midi[1].data_2 == 127);
    assert(midi[2].status == 0xB0);
    assert(midi[2].data_1 == 29);
    assert(midi[2].data_2 == 0);
    assert(midi[3].status == 0xB0);
    assert(midi[3].data_1 == 30);
    assert(midi[3].data_2 == 127);
    
    //exit chord mode, return to detune
    controller.onAllButtonsPressed();
    assert(controller.getCurrentMode() == DETUNE);
    //now switch to source EQ
    controller.onButtonEvent(1);
    assert(controller.getCurrentMode() == SOURCE_EQ);
    /* Test 4:
     Flex 1: CC23, value 0
     Flex 2: CC24, value 127
     Flex 3: CC25, value 0
     Flex 4: CC26, value 127
     */
    flex_data = {0, 1, -0.01, 1.6};
    controller.onFlexEvent(flex_data);
    for (int i = 0; i < 4; i++){
        midi[i] = controller.getLastCC(i);
    }
    assert(midi[0].status == 0xB0);
    assert(midi[0].data_1 == 23);
    assert(midi[0].data_2 == 0);
    assert(midi[1].status == 0xB0);
    assert(midi[1].data_1 == 24);
    assert(midi[1].data_2 == 127);
    assert(midi[2].status == 0xB0);
    assert(midi[2].data_1 == 25);
    assert(midi[2].data_2 == 0);
    assert(midi[3].status == 0xB0);
    assert(midi[3].data_1 == 26);
    assert(midi[3].data_2 == 127);
    
    std::cout<<"finished";
    return(0);
}

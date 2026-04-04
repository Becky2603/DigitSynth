#include "SynthController.hpp"

void SynthController::onButtonEvent(int index){
    
}

void SynthController::onFlexEvent(std::array<ExtensionData, 4> data){
    
}

void SynthController::onAllButtonsPressed(){
    
}

FlexSensor flex;
flex.registerCallback(controller.onFlexEvent); // will need to change this

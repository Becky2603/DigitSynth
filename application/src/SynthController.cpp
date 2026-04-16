#include "SynthController.hpp"

SynthController::SynthController(TLC59711& tlc)
: ledController(tlc);
{
    this->buttonDriver.registerSingleButtonCallback([this] (int index) {
    });
    
    this->flexSensor.registerCallback([this] (std::array<ExtensionData, 4> values){
    });

    this->flexSensor.begin();
}

SynthController::~SynthController() {
    std::cout << this->flexSensor.getNSamples() << std::endl;
}

void SynthController::registerMidiCallback(MidiCallback callback){
    midiCallback = callback;
}

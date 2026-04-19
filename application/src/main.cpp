
#include "SynthController.hpp"
#include "TLC59711.h"
#include "adc-driver.h"
#include "button-driver.h"
#include "flex-sensor.h"
#include "gpio.h"
#include "midi-driver.hpp"
#include <iostream>

int main() {
    gpio::setupGpio();
    
    auto bd = new button_driver::ButtonDriver();
    auto adc = new adc_driver::Ads1115Driver();
    auto fs = new flex_sensor::FlexSensor(static_cast<adc_driver::IAdcDriver *>(adc));
    adc = nullptr;
    auto md = new midi_driver::MidiDriver();
    auto tlc = new led_driver::TLC59711(17, 27);
    tlc->start();
    SynthController synth(
        static_cast<led_driver::ILedDriver *>(tlc),
        static_cast<button_driver::IButtonDriver *>(bd),
        static_cast<flex_sensor::IFlexSensor *>(fs),
        static_cast<midi_driver::IMidiDriver *>(md)
    );
    bd = nullptr; 
    fs = nullptr; 
    md = nullptr;
    tlc = nullptr;
    getchar();
    std::cout <<"stop\n";
    
    
    
    return 0;
}

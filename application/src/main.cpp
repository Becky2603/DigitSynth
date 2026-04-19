
#include "SynthController.hpp"
#include "TLC59711.h"
#include "adc-driver.h"
#include "button-driver.h"
#include "flex-sensor.h"
#include "gpio.h"
#include "midi-driver.hpp"
#include "voltage-scaler.h"
#include <condition_variable>
#include <iostream>
#include <mutex>

int main() {
    gpio::setupGpio();
    std::condition_variable c;
    std::mutex m;
    bool done = false;
    
    auto bd = new button_driver::ButtonDriver();
    bd->registerAllButtonsCallback([&c, &m, &done] () {
        std::unique_lock lock(m);
        done = true;
        lock.unlock();
        c.notify_all();
    });
    
    auto adc = new adc_driver::Ads1115Driver();
    auto vs = new voltage_scaler::VoltageScaler();
    auto fs = new flex_sensor::FlexSensor(static_cast<adc_driver::IAdcDriver *>(adc), static_cast<voltage_scaler::IVoltageScaler *>(vs));
    adc = nullptr;
    vs = nullptr; 
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
   
    {
        std::unique_lock lock(m);
        c.wait(lock, [&done] { return done; }); 
    }
    std::cout <<"stop\n";
    
    
    
    return 0;
}

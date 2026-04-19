
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
#include <memory>
#include <mutex>

int main() {
    gpio::setupGpio();
    std::condition_variable c;
    std::mutex m;
    bool done = false;
    
    auto bd = std::make_unique<button_driver::ButtonDriver>();
    bd->registerAllButtonsCallback([&c, &m, &done] () {
        std::unique_lock lock(m);
        done = true;
        lock.unlock();
        c.notify_all();
    });
    
    auto adc = new adc_driver::Ads1115Driver();
    auto vs = new voltage_scaler::VoltageScaler();
    auto tlc = led_driver::TLC59711(17, 27);
    tlc.start();
    SynthController synth(
        tlc,
        std::make_unique<button_driver::ButtonDriver>(),
        std::make_unique<flex_sensor::FlexSensor>(static_cast<adc_driver::IAdcDriver *>(adc), static_cast<voltage_scaler::IVoltageScaler *>(vs)),
        std::make_unique<midi_driver::MidiDriver>()
    );
   
    {
        std::unique_lock lock(m);
        c.wait(lock, [&done] { return done; }); 
    }
    std::cout <<"stop\n";
    
    return 0;
}

#include "synth-controller.hpp"
#include "tlc59711.hpp"
#include "adc-driver.hpp"
#include "button-driver.hpp"
#include "flex-sensor.hpp"
#include "gpio.hpp"
#include "midi-driver.hpp"
#include "patterns.hpp"
#include "voltage-scaler.hpp"
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
    
    auto tlc = led_driver::TLC59711("/dev/spidev0.0", 1'000'000);
    auto pattern = led_pattern::PatternRipple(tlc);
    tlc.start();
    SynthController synth(
        tlc,
        pattern,
        std::move(bd),
        std::make_unique<flex_sensor::FlexSensor>(
            std::make_unique<adc_driver::Ads1115Driver>(),
            std::make_unique<voltage_scaler::VoltageScaler>()
        ),
        std::make_unique<midi_driver::MidiDriver>()
    );
   

    getchar();
    
    std::cout <<"stop\n";
    
    return 0;
}
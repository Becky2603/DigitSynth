#include "button-driver.h"
#include "gpio.h"
#include <gpiod.hpp>
#include <thread>

using namespace button_driver;

ButtonDriver::ButtonDriver() : IButtonDriver() {
    for (ButtonIndex i = 0; i < (ButtonIndex) workers.size(); i++) {
        workers[i] = std::thread([&, i] () {
            while (this->running) {
                auto edge = gpio::blockUntilEdge(ButtonDriver::BUTTON_PINS[i], gpiod::line::edge::BOTH);
                
                if (!edge.has_value()) { continue; }
                
                bool val = edge.value() == gpiod::edge_event::event_type::RISING_EDGE;
                
                if (!val) { continue; }
                if (!this->buttonCallback.has_value()) { continue; }
                    
                this->buttonCallback.value()(i); 
            }
        });  
    }
}

ButtonDriver::~ButtonDriver() {
    this->running = false;
    gpio::cancelLineRequests();
    for (std::thread &t: this->workers) {
        if (t.joinable()) { t.join(); }
    }
}

void ButtonDriver::registerButtonCallback(ButtonCallback callback) {
    this->buttonCallback = callback;
}

void ButtonDriver::deregisterButtonCallback() {
    this->buttonCallback = {};
}

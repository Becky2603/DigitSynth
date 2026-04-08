#include "button-driver.h"
#include "gpio.h"
#include "types.h"
#include <gpiod.hpp>
#include <thread>

ButtonDriver::ButtonDriver() {
    for (size_t i = 0; i < workers.size(); i++) {
        workers[i] = std::thread([&, i] () {
            while (running) {
                auto edge = gpio::blockUntilEdge(ButtonDriver::BUTTON_PINS[i], gpiod::line::edge::BOTH);
                bool val = edge == gpiod::edge_event::event_type::RISING_EDGE;
                this->buttonStatuses[i] = val;
                
                
                if (!val) { continue; }
                if (!this->singleButtonCallback.has_value()) { continue; }
                    
                this->singleButtonCallback.value()(i); 
               
                if (!this->allButtonsCallback.has_value()) { continue; }
                bool allPressed = false;
                for (auto status : buttonStatuses) {
                    if (!status) { allPressed = false; break; }
                    allPressed = true;
                }
                
                if (allPressed) { this->allButtonsCallback.value()(); }
            }
        });  
    }
}

void ButtonDriver::registerAllButtonsCallback(std::function<void(void)> callback) {
    this->allButtonsCallback = callback;
}

void ButtonDriver::deregisterSingleButtonCallback() {
    this->singleButtonCallback = {};
}


void ButtonDriver::registerSingleButtonCallback(ButtonCallback callback) {
    this->singleButtonCallback = callback;
}

void ButtonDriver::deregisterAllButtonsCallback() {
    this->allButtonsCallback = {};
}

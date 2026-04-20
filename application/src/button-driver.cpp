#include "button-driver.h"
#include "gpio.h"
#include <gpiod.hpp>
#include <thread>

using namespace button_driver;

ButtonDriver::ButtonDriver() {
    for (ButtonIndex i = 0; i < (ButtonIndex) workers.size(); i++) {
        workers[i] = std::thread([&, i] () {
            while (this->running) {
                auto edge = gpio::blockUntilEdge(ButtonDriver::BUTTON_PINS[i], gpiod::line::edge::BOTH);
                
                if (!edge.has_value()) { continue; }
                
                bool val = edge.value() == gpiod::edge_event::event_type::RISING_EDGE;
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

ButtonDriver::~ButtonDriver() {
    this->running = false;
    gpio::cancelLineRequests();
    for (std::thread &t: this->workers) {
        if (t.joinable()) { t.join(); }
    }
}

void ButtonDriver::registerAllButtonsCallback(AllButtonsCallback callback) {
    this->allButtonsCallback = callback;
}

void ButtonDriver::deregisterSingleButtonCallback() {
    this->singleButtonCallback = {};
}


void ButtonDriver::registerSingleButtonCallback(SingleButtonCallback callback) {
    this->singleButtonCallback = callback;
}

void ButtonDriver::deregisterAllButtonsCallback() {
    this->allButtonsCallback = {};
}

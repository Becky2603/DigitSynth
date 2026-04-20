#include "LedController.hpp"
#include "ILedDriver.hpp"
#include "patterns.h"
#include <cstdio>

LedController::LedController(led_driver::ILedDriver& tlc, Pattern& pattern,
                             std::unordered_map<LfoShape, float> shapeBrightness)
    : _tlc(tlc), _pattern(pattern), shapeBrightness(shapeBrightness) {}

void LedController::update(ControlMode mode, bool lfoEnabled, LfoShape shape, std::array<float, 4> flexValues){
    (void) mode; 
    if (_ledPattern == RIPPLE) {
        if (!rippleRunning) {
            startRipple();
            rippleRunning = true;
        }
    }
    else {
        if (rippleRunning) { 
            stopRipple();
            rippleRunning = false;
        }
        // Status pattern independent of chord/normal mode
        // handle normal LED stuff
        led_driver::ILedDriver::Channels c{};

        // Left hand
        c[Led::L_pinky]  = 1; // not needed here
        c[Led::L_index]  = 1; // indicate normal mode
        if (lfoEnabled == true){
            c[Led::L_middle] = 1;
        }
        else { c[Led::L_middle] = 0; }
        c[Led::L_ring] = shapeBrightness.at(shape);

        // Right hand — flex brightness per finger
        c[Led::R_pinky]  = flexValues[3];
        c[Led::R_index]  = flexValues[0];
        c[Led::R_middle] = flexValues[1];
        c[Led::R_ring]   = flexValues[2];

        this->_tlc.update(c);
    }
}

void LedController::startRipple() {
    _pattern.start();
}

void LedController::stopRipple() {
    _pattern.stop();
}

void LedController::togglePattern() {
    if (_ledPattern == STATUS){
        _ledPattern = RIPPLE;
    }
    else {
        _ledPattern = STATUS;
    }
}

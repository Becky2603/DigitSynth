#include "LedController.hpp"
#include "ILedDriver.hpp"
#include <cstdio>

LedController::LedController(led_driver::ILedDriver *tlc, Pattern& ripple)
    : _tlc(tlc), _ripple(ripple) {}

void LedController::update(ControlMode mode, bool lfoEnabled, LfoShape shape, std::array<float, 4> flexValues){
    (void) mode; 
    if (pattern == RIPPLE) {
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
        switch(shape){
            case SIN:
                c[Led::L_ring] = 0;
                break;
            case SQR:
                c[Led::L_ring] = 0.5;
                break;
            case SH:
                c[Led::L_ring] = 1;
                break;
        }

        // Right hand — flex brightness per finger
        c[Led::R_pinky]  = flexValues[3];
        c[Led::R_index]  = flexValues[0];
        c[Led::R_middle] = flexValues[1];
        c[Led::R_ring]   = flexValues[2];

        this->_tlc.update(c);
    }
}

void LedController::startRipple() {
    _ripple.start();
}

void LedController::stopRipple() {
    _ripple.stop();
}

void LedController::togglePattern() {
    if (pattern == STATUS){
        pattern = RIPPLE;
    }
    else {
        pattern = STATUS;
    }
}

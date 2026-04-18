#include "LedController.hpp"
#include "ITLC59711.hpp"
#include <cstdio>

LedController::LedController(ITLC59711& tlc, Pattern& ripple)
    : _tlc(tlc), _ripple(ripple) {}

void LedController::update(ControlMode mode, bool lfoEnabled, LfoShape shape, std::array<float, 4> flexValues){
    (void)lfoEnabled;
    (void)shape;
    if (mode == CHORD) {
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
        // handle normal LED stuff
        TLC59711::Channels c{};

        // Left hand — flex brightness per finger
        c[Led::L_pinky]  = flexValues[3];
        c[Led::L_index]  = flexValues[0];
        c[Led::L_middle] = flexValues[1];
        c[Led::L_ring]   = flexValues[2];

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

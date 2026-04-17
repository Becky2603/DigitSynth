#include "LedController.hpp"

LedController::LedController(ITLC59711& tlc, Pattern& ripple)
    : _tlc(tlc), _ripple(ripple) {}

void LedController::update(ControlMode mode, bool lfoEnabled, LfoShape shape, std::array<float, 4> flexValues){
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
        ITLC59711::Channels c{};
        c[0] = 1; // thumb LED on
        c[5] = 1; // thumb LED on
        c[6] = 1; // indicates normal mode
        if (lfoEnabled) { c[7] = 1;} // indicates LFO enabled/disabled
        else { c[7] = 0;}
        switch(shape){
            case SIN:
                c[8] = 0;
                break;
            case SQR:
                c[8] = 0.5;
                break;
            case SH:
                c[8] = 1;
                break;
        }
        c[9] = 1;
        c[1] = flexValues[0];
        c[2] = flexValues[1];
        c[3] = flexValues[2];
        c[4] = flexValues[3];
        this->_tlc.update(c);
    }
}

void LedController::startRipple() {
    _ripple.start();
}

void LedController::stopRipple() {
    _ripple.stop();
}

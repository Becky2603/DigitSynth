#ifndef _GPIO_H
#define _GPIO_H

#include "types.h"
#include <gpiod.hpp>

namespace gpio {
    void setupGpio();
    
    void setPin(int, bool value);
    bool getPin(int pin);
    gpiod::edge_event::event_type blockUntilEdge(int pin, gpiod::line::edge edge);
    void registerCallback(int pin, gpiod::line::edge edge, GpioCallback callback);
}

#endif

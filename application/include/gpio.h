#ifndef _GPIO_H
#define _GPIO_H

#include "types.h"
#include <gpiod.hpp>

namespace gpio {
    void setupGpio();
    void teardownGpio();
    
    void setPin(int, bool value);
    bool getPin(int pin);
    void blockUntilEdge(int pin, gpiod::line::edge edge);
}

#endif

#ifndef _GPIO_H
#define _GPIO_H

#include "types.h"
#include <gpiod.hpp>
#include <vector>
namespace gpio {
    void setupGpio();
    
    void setPin(int, bool value);
    bool getPin(int pin);
    void blockUntilEdge(int pin, gpiod::line::edge edge);
    void registerCallback(int pin, gpiod::line::edge edge, GpioCallback callback);
}

#endif

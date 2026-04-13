#ifndef _GPIO_H
#define _GPIO_H

#include "types.h"
#include <gpiod.hpp>

namespace gpio {
    /**
     * Set up the `gpiod` driver. *Must be called before any other `gpio` 
     * function is called.* 
     */
    void setupGpio();
    
    /**
     * Set a GPIO pin to a given value. 
     * @param pin --- the number (as it appears on the standard pinout) of the pin 
     *                to be set.
     * @param value --- the logic value to set the pin to. `false` => logic low. 
     */
    void setPin(int pin, bool value);
    
    /**
     * Read the logic level of a given pin. 
     * @param pin --- the pin to be read. 
     * @return The logic level of the pin. `false` => logic low.
     */
    bool getPin(int pin);
    
    /**
     * Blocks execution until the given edge transition occurs. 
     * @param pin --- the pin on which to block. 
     * @param edge --- the edge transition to wait for. 
     * @return  The event that occurred. Useful if `edge` is edge::BOTH and you wish
     *          to determine whether the edge was rising or falling. 
     */
    gpiod::edge_event::event_type blockUntilEdge(int pin, gpiod::line::edge edge);
    void registerCallback(int pin, gpiod::line::edge edge, GpioCallback callback);
}

#endif


#include "gpio.h"
#include <chrono>
#include <gpiod.hpp>
#include <thread>

bool testCancelLineRequests() {
    bool finished = false;
    std::thread t([&finished] () {
        gpio::blockUntilEdge(14, gpiod::line::edge::FALLING);
        finished = true; 
    });
    
    gpio::cancelLineRequests();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    if (t.joinable()) { t.join(); }
    
    return finished; 
}

int main() {
    gpio::setupGpio();
    
    bool success = true;
    
    success &= testCancelLineRequests();
    
    return success ? 0 : -1;
}

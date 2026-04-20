
#include "gpio.hpp"
#include <cassert>
#include <chrono>
#include <climits>
#include <gpiod.hpp>
#include <thread>
#include <unistd.h>
#include <limits.h>

void testCancelLineRequests() {
    bool finished = false;
    std::thread t([&finished] () {
        gpio::blockUntilEdge(14, gpiod::line::edge::FALLING);
        finished = true; 
    });
    
    gpio::cancelLineRequests();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    if (t.joinable()) { t.join(); }

    assert(finished);
}

int main() {
    char hostname[HOST_NAME_MAX];
    gethostname(hostname, HOST_NAME_MAX);
    std::cout << hostname << std::endl;
    auto hostname_string = std::string(hostname);
    if (hostname_string != "raspberrypi") { return 0; } // probably don't want to run GPIO operations if on a normal user machine 
        
    gpio::setupGpio();
    
    testCancelLineRequests();
    
    return 0;
}

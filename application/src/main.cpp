#include <iostream>
#include <thread>
#include <chrono>
#include "TLC59711.h"

#define DATA_PIN  17
#define CLK_PIN   27

int main() {
    try {
        TLC59711 leds(DATA_PIN, CLK_PIN);
        std::cout << "TLC59711 initialised\n";
        std::cout << "GPIO claimed — LED 0 toggle test\n";

        // we don't have buildPacket/shiftOut yet so just confirm
        // construction and destruction work cleanly
        std::cout << "Press enter to exit...\n";
        std::cin.get();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
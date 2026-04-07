#include <chrono>
#include <iostream>
#include <thread>
#include "TLC59711.h"
#include "patterns.h"

#define DATA_PIN 17
#define CLK_PIN  27

int main() {
    try {
        TLC59711 tlc(DATA_PIN, CLK_PIN);
        tlc.start();

        // std::cout << "Fading in...\n";
        // pattern_fade(tlc);

        std::cout << "Ripple running...\n";
        pattern_ripple(tlc);

        tlc.stop();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
#include <iostream>
#include <thread>
#include <chrono>
#include "TLC59711.h"
#include "patterns.h"

#define DATA_PIN 17
#define CLK_PIN  27

int main() {
    try {
        TLC59711 tlc(DATA_PIN, CLK_PIN);
        tlc.start();
        std::cout << "Ripple running for 30 seconds...\n";

        std::thread pattern_thread([&]() {
            pattern_ripple(tlc);
        });

        std::this_thread::sleep_for(std::chrono::seconds(30));
        tlc.stop();
        pattern_thread.detach();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
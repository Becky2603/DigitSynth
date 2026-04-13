#include "SynthController.hpp"
#include "TLC59711.h"

#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

static bool g_running = true;

void signalHandler(int) {
    g_running = false;
}

int main() {
    std::signal(SIGINT, signalHandler);

    // Replace these with your real GPIO pins / driver count
    constexpr int DATA_PIN = 17;
    constexpr int CLK_PIN  = 27;

    try {
        TLC59711 tlc(DATA_PIN, CLK_PIN);
        tlc.start();

        SynthController synth(tlc);

        std::cout << "Starting LED integration test...\n";

        // Give the TLC worker a moment to start
        std::this_thread::sleep_for(500ms);

        // Assumed button mapping:
        // 0 -> EQ
        // 1 -> SOURCE_EQ
        // 2 -> DETUNE
        // 3 -> CHORD
        //
        // Adjust these if your ModeManager uses different indices.

        std::cout << "\nPressing button 1: enter SOURCE_EQ\n";
        synth.onButtonEvent(1);
        std::cout << "Expected: ripple pattern\n";
        std::this_thread::sleep_for(4s);

        std::cout << "\nPressing button 0: switch to EQ\n";
        synth.onButtonEvent(0);
        std::cout << "Expected: ripple stops, fade starts\n";
        std::this_thread::sleep_for(6s);

        std::cout << "\nPressing button 2: switch to DETUNE\n";
        synth.onButtonEvent(2);
        std::cout << "Expected: fade stops, ripple starts\n";
        std::this_thread::sleep_for(4s);

        std::cout << "\nPressing button 0: switch back to EQ\n";
        synth.onButtonEvent(0);
        std::cout << "Expected: ripple stops, fade starts\n";
        std::this_thread::sleep_for(6s);

        std::cout << "\nPressing button 3: enter CHORD mode\n";
        synth.onButtonEvent(3);
        std::cout << "Expected: currently no LED change in your code\n";
        std::this_thread::sleep_for(3s);

        std::cout << "\nIn CHORD mode, pressing button 1\n";
        synth.onButtonEvent(1);
        std::cout << "Expected: MIDI chord update only, no LED change yet\n";
        std::this_thread::sleep_for(2s);

        std::cout << "\nTest complete. Press Ctrl+C to exit early next time.\n";

        tlc.stop();
    }
    catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << '\n';
        return 1;
    }

    return 0;
}

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include "midi-driver.hpp"
#include "MidiTypes.hpp"

int main() {
    try {
        MidiDriver midi;

        auto ports = midi.listOutputPorts();

        std::cout << "Available MIDI output ports:\n";
        for (size_t i = 0; i < ports.size(); ++i) {
            std::cout << i << ": " << ports[i] << '\n';
        }

        if (ports.empty()) {
            std::cerr << "No MIDI output ports found.\n";
            return 1;
        }

        std::cout << "Opening port 0...\n";
        midi.openPort(0);

        midi_message note_on  {0x90, 60, 100}; // Note on, channel 1, middle C
        midi_message note_off {0x80, 60, 0};   // Note off, channel 1, middle C

        std::cout << "Sending NOTE ON...\n";
        midi.sendMessage(note_on);

        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        std::cout << "Sending NOTE OFF...\n";
        midi.sendMessage(note_off);

        std::cout << "Done.\n";
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "MIDI test failed: " << e.what() << '\n';
        return 1;
    }
}
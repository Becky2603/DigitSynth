#include <chrono>
#include <iostream>
#include <thread>
#include <array>

#include "SynthController.hpp"
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

        // --- Synth setup ---
        TLC59711 tlc(17, 27);
        SynthController synth(tlc);

        // Register callback: synth → MIDI driver
        synth.registerMidiCallback(
            [&](const midi_message& msg) {
                midi.sendMessage(msg);
            }
        );

        // --- Test flex input ---
        std::array<ExtensionData, 4> flex_data = {0.2, 0.5, 0.8, 1.0};

        std::cout << "Sending flex event (should produce CC messages)...\n";
        synth.onFlexEvent(flex_data);

        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        std::cout << "Done.\n";
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Flex MIDI test failed: " << e.what() << '\n';
        return 1;
    }
}

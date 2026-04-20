#include "midi-driver.hpp"
#include "MidiTypes.hpp"

#include <iostream>
#include <thread>
#include <chrono>

// MIDI status bytes
static constexpr unsigned char NOTE_ON  = 0x90; // channel 1
static constexpr unsigned char NOTE_OFF = 0x80; // channel 1
static constexpr unsigned char CC       = 0xB0; // channel 1

// JD-Xi analog synth is ch3 (0x92/0x82), digital part 1 is ch1 (0x90/0x80)
// We'll use digital part 1 for this test

void playNote(MidiDriver& driver, unsigned char note, unsigned char velocity, int durationMs) {
    midi_message noteOn  = { NOTE_ON,  note, velocity };
    midi_message noteOff = { NOTE_OFF, note, 0        };

    driver.sendMessage(noteOn);
    std::this_thread::sleep_for(std::chrono::milliseconds(durationMs));
    driver.sendMessage(noteOff);
}

int main() {
    MidiDriver driver;

    // 1. List available ports
    std::cout << "Available MIDI output ports:\n";
    auto ports = driver.listOutputPorts();
    if (ports.empty()) {
        std::cerr << "No MIDI output ports found. Is the JD-Xi plugged in?\n";
        return 1;
    }
    for (size_t i = 0; i < ports.size(); i++) {
        std::cout << "  [" << i << "] " << ports[i] << "\n";
    }

    // 2. Find the JD-Xi port (or fall back to port 0)
    int targetPort = 0;
    for (size_t i = 0; i < ports.size(); i++) {
        if (ports[i].find("JD-Xi") != std::string::npos ||
            ports[i].find("Roland") != std::string::npos) {
            targetPort = static_cast<int>(i);
            std::cout << "Found JD-Xi at port " << i << "\n";
            break;
        }
    }

    // 3. Open the port
    std::cout << "Opening port " << targetPort << " (" << ports[targetPort] << ")...\n";
    try {
        driver.openPort(static_cast<unsigned int>(targetPort));
    } catch (const std::exception& e) {
        std::cerr << "Failed to open port: " << e.what() << "\n";
        return 1;
    }
    std::cout << "Port opened successfully.\n";

    // 4. Send a CC message to set filter cutoff (CC 74) to full open
    std::cout << "Sending CC 74 (filter cutoff) = 127...\n";
    midi_message cc = { CC, 74, 127 };
    driver.sendMessage(cc);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // 5. Play a simple C major arpeggio: C4, E4, G4, C5
    std::cout << "Playing C major arpeggio...\n";
    unsigned char notes[] = { 60, 64, 67, 72 };
    for (unsigned char note : notes) {
        std::cout << "  Note " << static_cast<int>(note) << "\n";
        playNote(driver, note, 100, 400);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "Test complete.\n";
    return 0;
}

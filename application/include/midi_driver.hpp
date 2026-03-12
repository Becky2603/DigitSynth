#pragma once

/**
 * MIDI Driver (Simple CC Output)
 *
 * This file implements a small MIDI output driver written in C++.
 * The driver uses the RtMidi library to send MIDI Control Change (CC)
 * messages from our program to other MIDI software on the system.
 *
 * The driver is header-only
 *
 * Example usage:
 *
 *   MidiDriver midi;
 *   midi.sendCC(0, 1, 64);   // channel 1, CC 1, value 64
 *
 * When the program runs it creates a virtual MIDI port, which can then
 * be connected to other software such as a DAW or synthesiser.
 *
 * Dependencies:
 *   RtMidi library
 *   Install with:
 *   sudo apt install librtmidi-dev
 *
 * Compile flag:
 *   -lrtmidi
 */

#include <RtMidi.h>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vector>

class MidiDriver {
public:

    /**
     * Constructor
     *
     * Creates a virtual MIDI output port
     *
     * default port name is "RPi MIDI CC Driver", but can be
     * changed if needed.
     */
    explicit MidiDriver(const std::string& port_name = "RPi MIDI CC Driver") {
        midi_.openVirtualPort(port_name);
        std::cout << "[MIDI] Virtual port opened: \"" << port_name << "\"\n";
    }

    /**
     * sendCC()
     *
     * Sends a MIDI Control Change (CC) message.
     *
     * Parameters:
     *   channel : MIDI channel number (0–15)
     *             Note: 0 corresponds to MIDI channel 1
     *
     *   cc      : Control Change number (0–127)
     *
     *   value   : Value for the control (0–127)
     *
     * Internally this builds the 3-byte MIDI message and sends it using RtMidi.
     */
    void sendCC(uint8_t channel, uint8_t cc, uint8_t value) {
        msg_[0] = 0xB0 | (channel & 0x0F);
        msg_[1] = cc    & 0x7F;
        msg_[2] = value & 0x7F;
        midi_.sendMessage(&msg_);
    }

private:
    RtMidiOut            midi_;          // RtMidi object used to send messages
    std::vector<uint8_t> msg_{0, 0, 0};  // 3-byte MIDI message buffer
};     

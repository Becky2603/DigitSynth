#include "midi-driver.hpp"
#include "types.h"

#include <rtmidi/RtMidi.h>
#include <string>
#include <stdexcept>

MidiDriver::MidiDriver()
    : midiOut_(rt::midi::RtMidi::Api::LINUX_ALSA), portOpen_(false) { 

    }

std::vector<std::string> MidiDriver::listOutputPorts() {
    std::vector<std::string> ports;
    const unsigned int count = midiOut_.getPortCount();

    ports.reserve(count);
    for (unsigned int i = 0; i < count; i++) {
        ports.push_back(midiOut_.getPortName(i));
    }

    return ports;
}

void MidiDriver::openPort(unsigned int portIndex) {
    const unsigned int count = midiOut_.getPortCount();
    if (portIndex >= count) {
        throw std::out_of_range("Requested MIDI port index does not exist");
    }

    midiOut_.openPort(portIndex);
    portOpen_ = true;
}

void MidiDriver::sendMessage(const midi_message& msg) {
    if (!portOpen_) {
        throw std::runtime_error("MIDI output port is not open");
    }
    
    std::vector<unsigned char> bytes = {
        msg.status,
        msg.data_1,
        msg.data_2
    };

    midiOut_.sendMessage(&bytes);
}

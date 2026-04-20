#include "midi-driver.hpp"

#include <rtmidi/RtMidi.h>
#include <string>
#include <stdexcept>

using namespace midi_driver;

MidiDriver::MidiDriver()
    : midiOut_(rt::midi::RtMidi::Api::LINUX_ALSA), portOpen_(false) {}


//Returns names of all currently available MIDI output ports
std::vector<std::string> MidiDriver::listOutputPorts() {
    std::vector<std::string> ports;
    const unsigned int count = midiOut_.getPortCount();

    ports.reserve(count);
    for (unsigned int i = 0; i < count; i++) {
        ports.push_back(midiOut_.getPortName(i));
    }

    return ports;
}

//Opens MIDI output port at the given index. Must be called before sendMessage()
void MidiDriver::openPort(unsigned int portIndex) {
    const unsigned int count = midiOut_.getPortCount();
    if (portIndex >= count) {
        throw std::out_of_range("Requested MIDI port index does not exist");
    }

    midiOut_.openPort(portIndex);
    portOpen_ = true;
}

//Sends 3-byte MIDI message to the open output port. 
//msg.status carries the MIDI status byte (e.g. 0x90 = Note on, channel 1)
//msg.data_1 and msg.data_2 carry the message payload (e.g. note, velocity)
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

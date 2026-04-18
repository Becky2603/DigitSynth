#ifndef MIDI_DRIVER_HPP
#define MIDI_DRIVER_HPP

#include <string>
#include <vector>
#include <rtmidi/RtMidi.h>

#include "MidiTypes.hpp"

class MidiDriver {
    public:
        MidiDriver();

        MidiDriver(const MidiDriver&) = delete;
        MidiDriver& operator=(const MidiDriver&) = delete;

        std::vector<std::string> listOutputPorts();
        void openPort(unsigned int portIndex);

        void sendMessage(const midi_message& msg);

    private:
        rt::midi::RtMidiOut midiOut_;
        bool portOpen_;
};

#endif

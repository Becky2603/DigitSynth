#ifndef MIDI_DRIVER_HPP
#define MIDI_DRIVER_HPP

#include <string>
#include <vector>

#include "MidiTypes.hpp"
#include "types.h"

class RtMidiOut;

class MidiDriver {
    public:
        MidiDriver();
        ~MidiDriver();

        MidiDriver(const MidiDriver&) = delete;
        MidiDriver& operator=(const MidiDriver&) = delete;

        std::vector<std::string> listOutputPorts() const;
        void openPort(unsigned int portIndex);

        void sendMessage(const midi_message& msg);

        // Returns a callback you can hand directly to
        // SynthController::registerMidiCallback()
        MidiCallback makeCallback();

    private:
        RtMidiOut* midiOut_;
        bool portOpen_;
};

#endif
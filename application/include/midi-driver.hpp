#ifndef MIDI_DRIVER_HPP
#define MIDI_DRIVER_HPP

#include <string>
#include <vector>

#include "MidiTypes.hpp"

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
        void noteOnCallback(const midi_message& msg);
        void ccCallback(const midi_message& msg);

    private:
        RtMidiOut* midiOut_;
        bool portOpen_;
};

#endif
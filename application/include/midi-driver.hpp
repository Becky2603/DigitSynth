#ifndef MIDI_DRIVER_HPP
#define MIDI_DRIVER_HPP

#include <string>
#include <vector>
#include <rtmidi/RtMidi.h>

#include "midi-types.hpp"

namespace midi_driver {
    
class IMidiDriver {
public: 
    virtual ~IMidiDriver() = default;
    
    virtual std::vector<std::string> listOutputPorts() = 0;
    virtual void openPort(unsigned int) = 0;
    virtual void sendMessage(const midi_message&) = 0;
};

class MidiDriver : public IMidiDriver {
public:
    MidiDriver();

    MidiDriver(const MidiDriver&) = delete;
    MidiDriver& operator=(const MidiDriver&) = delete;

    std::vector<std::string> listOutputPorts() override;
    void openPort(unsigned int portIndex) override;

    void sendMessage(const midi_message& msg) override;

private:
    rt::midi::RtMidiOut midiOut_;
    bool portOpen_;
};

}

#endif

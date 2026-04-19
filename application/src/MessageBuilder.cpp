#include "MessageBuilder.hpp"

std::vector<midi_message> MessageBuilder::buildMessages(int index, uint8_t value) {
    switch (index) {
        case 0: // filter cutoff — standard CC 74
            return buildCC(74, value);
        case 1: // LFO rate — NRPN LSB 6
            return buildNRPN(6, value);
        case 2: // LFO filter depth — NRPN LSB 18
            return buildNRPN(18, value);
        case 3: // pitch bend
            return buildPitchBend(value);
        default:
            return {};
    }
}

std::vector<midi_message> MessageBuilder::buildCC(uint8_t cc, uint8_t value) {
    return {{ 0xB0, cc, value }};
}

std::vector<midi_message> MessageBuilder::buildNRPN(uint8_t lsb, uint8_t value) {
    return {
        { 0xB0, 99, 0   },  // NRPN MSB
        { 0xB0, 98, lsb },  // NRPN LSB
        { 0xB0,  6, value}, // Data entry MSB
        { 0xB0, 38, 0   },  // Data entry LSB
    };
}

std::vector<midi_message> MessageBuilder::buildPitchBend(uint8_t value) {
    return {{ 0xE0, 0, value }};
}

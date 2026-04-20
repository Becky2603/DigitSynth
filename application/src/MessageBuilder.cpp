#include "MessageBuilder.hpp"

std::vector<midi_message> MessageBuilder::buildMessages(int index, uint8_t value) {
    if (index < 0 || index >= static_cast<int>(builders.size())) return {};
    return builders[index](value);
}

std::vector<midi_message> MessageBuilder::buildCC(uint8_t cc, uint8_t value) {
    return {{ 0xB0, cc, value }};
}

std::vector<midi_message> MessageBuilder::buildNRPN(uint8_t lsb, uint8_t value) {
    return {
        { 0xB0, 99, 0    },
        { 0xB0, 98, lsb  },
        { 0xB0,  6, value},
        { 0xB0, 38, 0    },
    };
}

std::vector<midi_message> MessageBuilder::buildPitchBend(uint8_t value) {
    return {{ 0xE0, 0, value }};
}

#ifndef MessageBuilder_hpp
#define MessageBuilder_hpp

#include <cstdint>
#include <vector>
#include <functional>
#include "midi-types.hpp"

/**
 * @class MessageBuilder
 * @brief Constructs MIDI messages based on input index and value.
 *
 * This class provides a high-level interface for generating different types
 * of MIDI messages (e.g., Control Change, NRPN, Pitch Bend) depending on
 * which control input is being used.
 */
class MessageBuilder {
public:
    /**
     * @brief Build MIDI messages for a given control index and value.
     *
     * Selects the appropriate message building function based on the index
     * and returns the resulting MIDI messages.
     *
     * @param index Index of the control input (e.g., flex sensor).
     * @param value Value associated with the control input.
     * @return std::vector<midi_message> A list of generated MIDI messages.
     */
    std::vector<midi_message> buildMessages(int index, uint8_t value);

private:
    /**
     * @brief Build a MIDI Control Change (CC) message.
     *
     * @param cc Control Change number.
     * @param value Value to assign to the CC.
     * @return std::vector<midi_message> Generated MIDI message(s).
     */
    std::vector<midi_message> buildCC(uint8_t cc, uint8_t value);

    /**
     * @brief Build a MIDI NRPN (Non-Registered Parameter Number) message.
     *
     * @param lsb Least significant byte identifying the NRPN parameter.
     * @param value Value to assign to the parameter.
     * @return std::vector<midi_message> Generated MIDI message(s).
     */
    std::vector<midi_message> buildNRPN(uint8_t lsb, uint8_t value);

    /**
     * @brief Build a MIDI Pitch Bend message.
     *
     * @param value Pitch bend value.
     * @return std::vector<midi_message> Generated MIDI message(s).
     */
    std::vector<midi_message> buildPitchBend(uint8_t value);

    /**
     * @brief Mapping of control indices to message-building functions.
     *
     * Each lambda corresponds to a specific control input and defines
     * how its value should be translated into MIDI messages.
     */
    const std::vector<std::function<std::vector<midi_message>(uint8_t)>> builders = {
        [this](uint8_t v){ return buildCC(74, v); },     ///< Filter cutoff (CC 74)
        [this](uint8_t v){ return buildNRPN(6, v); },    ///< LFO rate (NRPN 6)
        [this](uint8_t v){ return buildNRPN(18, v); },   ///< LFO depth (NRPN 18)
        [this](uint8_t v){ return buildPitchBend(v); }   ///< Pitch bend
    };
};

#endif

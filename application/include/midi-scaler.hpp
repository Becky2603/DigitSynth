#ifndef MidiScaler_hpp
#define MidiScaler_hpp

#include <cstdint>

/**
 * Scales normalised float sensor readings to the MIDI value range.
 *
 * Converts a float in the range [0.0, 1.0] to a uint8_t in the range [0, 127],
 * as required by MIDI CC, velocity, and other 7-bit parameters.
 */
class MidiScaler {
public:
    /**
     * Scales a normalised float value to the MIDI range [0, 127].
     *
     * Values below 0.0 are clamped to 0 and values above 1.0 are clamped to 127.
     *
     * @param value Input value in range [0.0, 1.0]
     * @return Scaled MIDI value in range [0, 127]
     */
    uint8_t scaleValue(float value);
};

#endif /* MidiScaler_hpp */

#ifndef ChordManager_hpp
#define ChordManager_hpp

#include <cstdint>

/**
 * Manages chord selection and note lookup for the DigitSynth
 *
 * Stores a fixed set of four chords (Fmaj9, Am11, Gsus2, Cmaj7),
 * each voiced across six notes spanning two octaves. The active chord is selected
 * by index
 */
class ChordManager {
public:
    /**
     * Sets the active chord by index
     *
     * @param index Chord index in range [0, 3]:
     *              0 = Fmaj9, 1 = Am11, 2 = Gsus2, 3 = Cmaj7
     */
    void updateChord(int index);

    /**
     * Returns the index of the currently active chord.
     *
     * @return Current chord index in range [0, 3]
     */
    uint8_t getCurrentChord();

    /**
     * Returns a single note from the current chord
     *
     * Each chord contains 6 notes. Call this 6 times (i = 0..5) to retrieve
     * the full voicing, from bass note up to the highest voice.
     *
     * @param i Note index in range [0, 5], where 0 is the bass note
     * @return MIDI note number (0–127)
     */
    uint8_t getNote(int i);

private:
    uint8_t currentChordIndex = 1;
    const uint8_t noteSelection[24] = {
        41, 53, 57, 60, 64, 67,  // Fmaj9
        33, 45, 52, 55, 62, 67,  // Am11
        31, 43, 50, 57, 62, 67,  // Gsus2
        36, 48, 55, 59, 64, 67   // Cmaj7
    };
};

#endif /* ChordManager_hpp */

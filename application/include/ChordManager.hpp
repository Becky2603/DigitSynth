#ifndef ChordManager_hpp
#define ChordManager_hpp

#include <cstdint>

class ChordManager {
public:
    void updateChord(int index);
    uint8_t getCurrentChord();
    uint8_t getNote(int i);
private:
    uint8_t currentChordIndex = 0;
    const uint8_t noteSelection[24] = {
        41, 53, 57, 60, 64, 67,  //Fmaj9
        33, 45, 52, 55, 62, 67,  //Am11
        31, 43, 50, 57, 62, 67,  //Gsus2
        36, 48, 55, 59, 64, 67   //Cmaj9
    };
};

#endif /* ChordManager_hpp */


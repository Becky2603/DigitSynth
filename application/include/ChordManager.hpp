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
    uint8_t previousChordIndex = 0;
    const uint8_t noteSelection[16] = {57, 60, 64, 67, 50, 54, 57, 60, 55, 59, 62, 66, 48, 52, 55, 59}; //Am7, D7, GM7, CM7
}

#endif /* ChordManager_hpp */


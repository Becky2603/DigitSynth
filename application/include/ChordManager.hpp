//
//  ChordManager.hpp
//  digitSynth
//
//  Created by Finn McConville on 30/03/2026.
//

#ifndef ChordManager_hpp
#define ChordManager_hpp

#include <stdio.h>

class ChordManager {
public:
    void updateChord(int index);
    uint8_t getCurrentChord();
private:
    uint8_t currentChordIndex;
}

#endif /* ChordManager_hpp */

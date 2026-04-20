#include <cassert>
#include "ChordManager.hpp"

int main(){
    ChordManager chordMan;
    
    //initial chord index = 1
    uint8_t chord1 = chordMan.getCurrentChord();
    assert(chord1 == 1);
    uint8_t test[6];
    for (int i = 0; i < 6; i++){
        test[i] = chordMan.getNote(i);
    }
    assert(test[0] == 33);
    assert(test[1] == 45);
    assert(test[2] == 52);
    assert(test[3] == 55);
    assert(test[4] == 62);
    assert(test[5] == 67);
    
    //2nd chord: chordIndex = 0
    chordMan.updateChord(0);
    uint8_t chord2 = chordMan.getCurrentChord();
    assert(chord2 == 0);
    for (int i = 0; i < 6; i++){
        test[i] = chordMan.getNote(i);
    }
    assert(test[0] == 41);
    assert(test[1] == 53);
    assert(test[2] == 57);
    assert(test[3] == 60);
    assert(test[4] == 64);
    assert(test[5] == 67);
    
    
    //3rd chord: chordIndex = 2
    chordMan.updateChord(2);
    uint8_t chord3 = chordMan.getCurrentChord();
    assert(chord3 == 2);
    for (int i = 0; i < 6; i++){
        test[i] = chordMan.getNote(i);
    }
    assert(test[0] == 31);
    assert(test[1] == 43);
    assert(test[2] == 50);
    assert(test[3] == 57);
    assert(test[4] == 62);
    assert(test[5] == 67);
    
    //4th chord: chordIndex = 3
    chordMan.updateChord(3);
    uint8_t chord4 = chordMan.getCurrentChord();
    assert(chord4 == 3);
    for (int i = 0; i < 6; i++){
        test[i] = chordMan.getNote(i);
    }
    assert(test[0] == 36);
    assert(test[1] == 48);
    assert(test[2] == 55);
    assert(test[3] == 59);
    assert(test[4] == 64);
    assert(test[5] == 67);
    
    return(0);
}

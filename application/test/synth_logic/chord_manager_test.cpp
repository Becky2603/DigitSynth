#include <cassert>
#include "ChordManager.hpp"

int main(){
    ChordManager chordMan;
    
    //1st chord: chordIndex = 0
    uint8_t chord1 = chordMan.getCurrentChord();
    assert(chord1 == 0);
    uint8_t test[4];
    for (int i = 0; i < 4; i++){
        test[i] = chordMan.getNote(i);
    }
    assert(test[0] == 57);
    assert(test[1] == 60);
    assert(test[2] == 64);
    assert(test[3] == 67);
    
    //2nd chord: chordIndex = 1
    chordMan.updateChord(1);
    uint8_t chord2 = chordMan.getCurrentChord();
    assert(chord2 == 1);
    for (int i = 0; i < 4; i++){
        test[i] = chordMan.getNote(i);
    }
    assert(test[0] == 50);
    assert(test[1] == 54);
    assert(test[2] == 57);
    assert(test[3] == 60);
    
    //3rd chord: chordIndex = 2
    chordMan.updateChord(2);
    uint8_t chord3 = chordMan.getCurrentChord();
    assert(chord3 == 2);
    for (int i = 0; i < 4; i++){
        test[i] = chordMan.getNote(i);
    }
    assert(test[0] == 55);
    assert(test[1] == 59);
    assert(test[2] == 62);
    assert(test[3] == 66);
    
    //4th chord: chordIndex = 3
    chordMan.updateChord(3);
    uint8_t chord4 = chordMan.getCurrentChord();
    assert(chord4 == 3);
    for (int i = 0; i < 4; i++){
        test[i] = chordMan.getNote(i);
    }
    assert(test[0] == 48);
    assert(test[1] == 52);
    assert(test[2] == 55);
    assert(test[3] == 59);
    
    return(0);
}

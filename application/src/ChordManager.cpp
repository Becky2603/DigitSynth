#include "ChordManager.hpp"

void ChordManager::updateChord(int index){
    switch(index){
        case 0:
            currentChordIndex = 0;
            break;
        case 1:
            currentChordIndex = 1;
            break;
        case 2:
            currentChordIndex = 2;
            break;
        case 3:
            currentChordIndex = 3;
    }
}

uint8_t ChordManager::getCurrentChord(){
    return currentChordIndex;
}

uint8_t ChordManager::getNote(int i){
    // i loops 0-3 for the number of notes in the chord
    // this function gets called 4 times in total for outputting one chord's notes
    uint8_t note = noteSelection[(4 * currentChordIndex) + i]; // find the current index in the array of notes
    return note;
}


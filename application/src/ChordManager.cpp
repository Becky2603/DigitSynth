#include "ChordManager.hpp"

void ChordManager::updateChord(int index){
    currentChordIndex = index;
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


#include "chord-manager.hpp"


void ChordManager::updateChord(int index){
    currentChordIndex = index;
}

uint8_t ChordManager::getCurrentChord(){
    return currentChordIndex;
}

uint8_t ChordManager::getNote(int i){
    // i loops 0-5 for the number of notes in the chord
    // this function gets called 6 times in total for outputting one chord's notes
    uint8_t note = noteSelection[(6 * currentChordIndex) + i]; // find the current index in the array of notes
    return note;
}


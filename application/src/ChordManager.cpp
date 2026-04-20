#include "ChordManager.hpp"

/**
 * updateChord changes the current chord index to the given index, which is used to determine which notes to output
 * 
 */
void ChordManager::updateChord(int index){
    currentChordIndex = index;
}

/**
 * getCurrentChord returns the current chord index
 * 
 */
uint8_t ChordManager::getCurrentChord(){
    return currentChordIndex;
}

/**
 * getNote returns the note at the given index for the current chord
 * 
 */
uint8_t ChordManager::getNote(int i){
    // i loops 0-5 for the number of notes in the chord
    // this function gets called 6 times in total for outputting one chord's notes
    uint8_t note = noteSelection[(6 * currentChordIndex) + i]; // find the current index in the array of notes
    return note;
}


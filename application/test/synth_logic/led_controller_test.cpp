#include "LedController.hpp"
#include "MockPattern.hpp"
#include "MockTLC59711.hpp"
#include "MidiTypes.hpp"
#include <cassert>

int main(){
    MockPattern testPattern;
    MockTLC59711 mockTLC;
    LedController ledController(mockTLC, testPattern);
    
    //initially, pattern is not running
    assert(testPattern.startCalled == false);
    assert(testPattern.stopCalled == false);
    
    // test 1: entering chord mode starts the pattern
    ledController.update(CHORD, true, SIN, {0, 0, 0, 0});
    assert(testPattern.startCalled == true);
    assert(testPattern.stopCalled == false);
    
    // test 2: pattern doesn't restart if it's already running
    testPattern.startCalled = false;
    ledController.update(CHORD, true, SIN, {0, 0, 0, 0});
    assert(testPattern.startCalled == false);
    assert(testPattern.stopCalled == false);
    
    //test 3: returning to normal mode stops the pattern
    ledController.update(NORMAL, true, SIN, {0, 0, 0, 0});
    assert(testPattern.startCalled == false);
    assert(testPattern.stopCalled == true);
    //test 4: normal mode thumb LEDs always on
    assert(mockTLC.lastChannels[0] == 1);
    assert(mockTLC.lastChannels[5] == 1);
    
    //test 5: button 1 LED indicates normal mode
    assert(mockTLC.lastChannels[6] == 1);
    //test 6: button 2 LED indicates LFO enabled/disabled
    assert(mockTLC.lastChannels[7] == 1);
    ledController.update(NORMAL, false, SIN, {0, 0, 0, 0});
    assert(mockTLC.lastChannels[7] == 0);
    //test 7: button 3 LED brightness indicates LFO shape
    assert(mockTLC.lastChannels[8] == 0);
    ledController.update(NORMAL, false, SQR, {0, 0, 0, 0});
    assert(mockTLC.lastChannels[8] == 0.5);
    ledController.update(NORMAL, false, SH, {0, 0, 0, 0});
    assert(mockTLC.lastChannels[8] == 1);
    //test 8: flex sensor values drive LED brightnesses
    ledController.update(NORMAL, false, SQR, {0.3f, 0.68f, 0.11f, 0.314f});
    assert(mockTLC.lastChannels[1] == 0.3f);
    assert(mockTLC.lastChannels[2] == 0.68f);
    assert(mockTLC.lastChannels[3] == 0.11f);
    assert(mockTLC.lastChannels[4] == 0.314f);
    //test 9: button 4 LED always on when device is in normal mode
    assert(mockTLC.lastChannels[9] == 1);
    
    return 0;
}

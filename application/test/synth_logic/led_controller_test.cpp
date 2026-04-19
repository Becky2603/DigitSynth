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

    //test 4: flex sensor values drive left hand LED brightnesses
    ledController.update(NORMAL, false, SIN, {0.3f, 0.68f, 0.11f, 0.314f});
    assert(mockTLC.lastChannels[Led::L_pinky]  == 0.3f);
    assert(mockTLC.lastChannels[Led::L_index]  == 0.68f);
    assert(mockTLC.lastChannels[Led::L_middle] == 0.11f);
    assert(mockTLC.lastChannels[Led::L_ring]   == 0.314f);

    //test 5: same flex values mirrored to right hand LEDs
    assert(mockTLC.lastChannels[Led::R_pinky]  == 0.3f);
    assert(mockTLC.lastChannels[Led::R_index]  == 0.68f);
    assert(mockTLC.lastChannels[Led::R_middle] == 0.11f);
    assert(mockTLC.lastChannels[Led::R_ring]   == 0.314f);
    
    return 0;
}

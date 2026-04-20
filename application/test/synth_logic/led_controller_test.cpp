#include "led-controller.hpp"
#include "mock-pattern.hpp"
#include "mock-tlc59711.hpp"
#include "midi-types.hpp"
#include <cassert>

int main(){
    led_driver::MockTLC59711 mockTLC;
    MockPattern testPattern(mockTLC);
    LedController ledController(mockTLC, testPattern, {
        {SIN, 0.0f},
        {SQR, 0.5f},
        {SH,  1.0f}
    });

    //test 1: default pattern is STATUS, ripple not running;
    assert(testPattern.startCalled == false);
    assert(testPattern.stopCalled == false);

    //test 2: togglePattern starts ripple;
    ledController.togglePattern();
    ledController.update(NORMAL, true, SIN, {0,0,0,0});
    assert(testPattern.startCalled == true);
    assert(testPattern.stopCalled == false);

    //test 3: calling update again in RIPPLE pattern doesn't restart ripple;
    testPattern.startCalled = false;
    ledController.update(NORMAL, true, SIN, {0,0,0,0});
    assert(testPattern.startCalled == false);

    //test 4: togglePattern again stops ripple;
    ledController.togglePattern();
    ledController.update(NORMAL, true, SIN, {0,0,0,0});
    assert(testPattern.stopCalled == true);

    //test 5: in STATUS pattern, LFO enabled => L_middle = 1;
    ledController.update(NORMAL, true, SIN, {0,0,0,0});
    assert(mockTLC.lastChannels[Led::L_middle] == 1);

    //test 6: LFO disabled => L_middle = 0;
    ledController.update(NORMAL, false, SIN, {0,0,0,0});
    assert(mockTLC.lastChannels[Led::L_middle] == 0);

    //test 7: LFO shape reflected in L_ring brightness;
    ledController.update(NORMAL, true, SIN, {0,0,0,0});
    assert(mockTLC.lastChannels[Led::L_ring] == 0);
    ledController.update(NORMAL, true, SQR, {0,0,0,0});
    assert(mockTLC.lastChannels[Led::L_ring] == 0.5f);
    ledController.update(NORMAL, true, SH, {0,0,0,0});
    assert(mockTLC.lastChannels[Led::L_ring] == 1);

    //test 8: flex sensor values drive right hand LEDs correctly;
    ledController.update(NORMAL, true, SIN, {0.3f, 0.68f, 0.11f, 0.314f});
    assert(mockTLC.lastChannels[Led::R_index]  == 0.3f);
    assert(mockTLC.lastChannels[Led::R_middle] == 0.68f);
    assert(mockTLC.lastChannels[Led::R_ring]   == 0.11f);
    assert(mockTLC.lastChannels[Led::R_pinky]  == 0.314f);

    return 0;
};

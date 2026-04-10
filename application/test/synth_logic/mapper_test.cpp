#include <stdio.h>
#include "paramMapper.hpp"
#include "MidiTypes.hpp"
#include <cassert>

int main(){
    ParamMapper mapper;
    ControlMode mode = EQ;
    
    uint8_t test_1 = mapper.getCC(0, mode);
    assert(test_1 == 19);
    uint8_t test_2 = mapper.getCC(1, mode);
    assert(test_2 == 20);
    uint8_t test_3 = mapper.getCC(2, mode);
    assert(test_3 == 21);
    uint8_t test_4 = mapper.getCC(3, mode);
    assert(test_4 == 22);
    
    mode = SOURCE_EQ;
    uint8_t test_5 = mapper.getCC(1, mode);
    assert(test_5 == 24);
    
    mode = DETUNE;
    uint8_t test_6 = mapper.getCC(2, mode);
    assert(test_6 == 29);
    
    return 0;
}

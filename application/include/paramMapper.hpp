//
//  paramMapper.hpp
//  digitSynth
//
//  Created by Finn McConville on 30/03/2026.
//

#ifndef paramMapper_hpp
#define paramMapper_hpp

#include <stdio.h>

class ParamMapper {
public:
    uint8_t getCC(int index, ControlMode current_mode);
};

#endif /* paramMapper_hpp */

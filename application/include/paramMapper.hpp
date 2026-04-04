#ifndef paramMapper_hpp
#define paramMapper_hpp

#include <stdio.h>
#include <cstdint>

class ParamMapper {
public:
    uint8_t getCC(int index, ControlMode current_mode);
};

#endif /* paramMapper_hpp */

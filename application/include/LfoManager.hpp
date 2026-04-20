#ifndef LfoManager_hpp
#define LfoManager_hpp

#include "MidiTypes.hpp"
#include <vector>

class LfoManager {
public:
    void toggle();
    void cycleShape();
    bool isEnabled() const;
    LfoShape getShape() const;
private:
    bool enabled = true;
    const std::vector<LfoShape> shapes = {SIN, SQR, SH};
    int shapeIndex = 0;
};

#endif

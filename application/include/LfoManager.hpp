
#ifndef LfoManager_hpp
#define LfoManager_hpp

#include "MidiTypes.hpp"

class LfoManager {
public:
    void toggle();
    void cycleShape();
    bool isEnabled() const;
    LfoShape getShape() const;
private:
    bool enabled = true;
    LfoShape shape = SIN;
};

#endif /* LfoManager_hpp */

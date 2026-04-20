#include "lfo-manager.hpp"

void LfoManager::toggle(){
    enabled = !enabled;
}

void LfoManager::cycleShape(){
    shapeIndex = (shapeIndex + 1) % static_cast<int>(shapes.size());
}

bool LfoManager::isEnabled() const {
    return enabled;
}

LfoShape LfoManager::getShape() const {
    return shapes[shapeIndex];
}

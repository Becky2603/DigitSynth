#include "LfoManager.hpp"

void LfoManager::toggle(){
    enabled = !enabled;
}

void LfoManager::cycleShape(){
    switch(shape){
        case SIN:
            shape = SQR;
            break;
        case SQR:
            shape = SH;
            break;
        case SH:
            shape = SIN;
            break;
    }
}

bool LfoManager::isEnabled() const {
    return enabled;
}

LfoShape LfoManager::getShape() const {
    return shape;
}

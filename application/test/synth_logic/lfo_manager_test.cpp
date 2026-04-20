#include "lfo-manager.hpp"
#include <cassert>

int main(){
    LfoManager lfoManager;
    assert(lfoManager.isEnabled() == true);
    assert(lfoManager.getShape() == SIN);
    
    //cycle through the shapes
    lfoManager.cycleShape();
    assert(lfoManager.getShape() == SQR);
    lfoManager.cycleShape();
    assert(lfoManager.getShape() == SH);
    lfoManager.cycleShape();
    assert(lfoManager.getShape() == SIN);
    
    //disable
    lfoManager.toggle();
    assert(lfoManager.isEnabled() == false);
    //enable
    lfoManager.toggle();
    assert(lfoManager.isEnabled() == true);
    
    return 0;
}

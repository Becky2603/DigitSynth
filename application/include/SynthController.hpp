//
//  SynthController.hpp
//  digitSynth
//
//  Created by Finn McConville on 29/03/2026.
//

#ifndef SynthController_hpp
#define SynthController_hpp

class SynthController {
public:
    void onButtonEvent(int index); //callback for a single button press
    void onFlexEvent(int index, float value); //callback for a flex sensor update
    void onAllButtonsPressed(); //callback for every button pressed at same time
};

#endif /* SynthController_hpp */

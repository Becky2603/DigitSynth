#ifndef BUTTON_DRIVER_H_
#define BUTTON_DRIVER_H_

#include "types.h"
#include <thread>
#include <array>

class ButtonDriver {
public:
    ButtonDriver();
    
    void registerSingleButtonCallback(ButtonCallback);
    void deregisterSingleButtonCallback();
    void registerAllButtonsCallback(std::function<void(void)>);
    void deregisterAllButtonsCallback();

private: 
    static constexpr int N_BUTTONS = 4;
    static constexpr int BUTTON_PINS[N_BUTTONS] = { 14, 15, 18, 23 };

    std::array<std::thread, N_BUTTONS> workers; 
    std::array<bool, N_BUTTONS> buttonStatuses; 
    
    std::optional<ButtonCallback> singleButtonCallback;  
    std::optional<std::function<void(void)>> allButtonsCallback;  
    
    bool running = true; 
};

#endif

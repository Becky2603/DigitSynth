#ifndef BUTTON_DRIVER_H_
#define BUTTON_DRIVER_H_

#include "types.h"
#include <thread>
#include <array>
#include <optional>

class ButtonDriver {
public:
    ButtonDriver();
    ~ButtonDriver();
    
    /**
     * Register a callback to be called when a button is pressed. The index of the 
     * pressed button will be passed to the callback on call. 
     * @param callback --- the callback to be registered. If a callback is currently
     * registered, this will replace it. 
     */
    void registerSingleButtonCallback(SingleButtonCallback callback);
    
    /**
     * Clear the current single-button callback. 
     */
    void deregisterSingleButtonCallback();
    
    /**
     * Register a callback to be called when all buttons are pressed simultaneously.
     * @param callback --- the callback to be registered. If a callback is currently registered,
     * this will replace it. 
     */
    void registerAllButtonsCallback(AllButtonsCallback callback);
    
    /**
     * Clear the current all-button callback. 
     */
    void deregisterAllButtonsCallback();

private: 
    static constexpr int N_BUTTONS = 4;
    static constexpr int BUTTON_PINS[N_BUTTONS] = { 14, 15, 18, 23 };

    std::array<std::thread, N_BUTTONS> workers; 
    std::array<bool, N_BUTTONS> buttonStatuses; 
    
    std::optional<SingleButtonCallback> singleButtonCallback;  
    std::optional<AllButtonsCallback> allButtonsCallback;  
    
    bool running = true; 
};

#endif

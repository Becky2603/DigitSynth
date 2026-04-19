#ifndef BUTTON_DRIVER_H_
#define BUTTON_DRIVER_H_

#include <thread>
#include <array>
#include <functional>
#include <optional>

namespace button_driver {
    
using ButtonIndex = size_t;
using SingleButtonCallback = std::function<void(ButtonIndex)>;
using AllButtonsCallback   = std::function<void(void)>; 
    
class IButtonDriver {
public:  
    virtual ~IButtonDriver() = default;
    
    virtual void registerSingleButtonCallback(SingleButtonCallback callback) = 0;
    virtual void deregisterSingleButtonCallback() = 0;
    virtual void registerAllButtonsCallback(AllButtonsCallback callback) = 0;
    virtual void deregisterAllButtonsCallback() = 0;
};

class ButtonDriver : public IButtonDriver {
public:
    
    ButtonDriver();
    ~ButtonDriver();
    
    /**
     * Register a callback to be called when a button is pressed. The index of the 
     * pressed button will be passed to the callback on call. 
     * @param callback --- the callback to be registered. If a callback is currently
     * registered, this will replace it. 
     */
    void registerSingleButtonCallback(SingleButtonCallback callback) override;
    
    /**
     * Clear the current single-button callback. 
     */
    void deregisterSingleButtonCallback() override;
    
    /**
     * Register a callback to be called when all buttons are pressed simultaneously.
     * @param callback --- the callback to be registered. If a callback is currently registered,
     * this will replace it. 
     */
    void registerAllButtonsCallback(AllButtonsCallback callback) override;
    
    /**
     * Clear the current all-button callback. 
     */
    void deregisterAllButtonsCallback() override;

private: 
    static constexpr int N_BUTTONS = 4;
    static constexpr int BUTTON_PINS[N_BUTTONS] = { 14, 15, 18, 23 };

    std::array<std::thread, N_BUTTONS> workers; 
    std::array<bool, N_BUTTONS> buttonStatuses; 
    
    std::optional<SingleButtonCallback> singleButtonCallback;  
    std::optional<AllButtonsCallback> allButtonsCallback;  
    
    bool running = true; 
};

}

#endif

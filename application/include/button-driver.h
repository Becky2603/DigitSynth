#ifndef BUTTON_DRIVER_H_
#define BUTTON_DRIVER_H_

#include <thread>
#include <array>
#include <functional>
#include <optional>

namespace button_driver {
    using ButtonIndex = int;                   
    using ButtonCallback = std::function<void(ButtonIndex)>;   
       

class IButtonDriver {
public:
    virtual ~IButtonDriver() = default;

    virtual void registerButtonCallback(ButtonCallback callback);  
    virtual void deregisterButtonCallback();  
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
    void registerButtonCallback(ButtonCallback callback);
    
    /**
     * Clear the current button callback. 
     */
    void deregisterButtonCallback();
   
private: 
    static constexpr int N_BUTTONS = 4;
    static constexpr int BUTTON_PINS[N_BUTTONS] = { 14, 15, 18, 23 };

    std::array<std::thread, N_BUTTONS> workers; 
    
    std::optional<ButtonCallback> buttonCallback;  
    
    bool running = true; 
};

}
#endif

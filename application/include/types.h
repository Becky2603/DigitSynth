#ifndef CALLBACK_TYPES_H_
#define CALLBACK_TYPES_H_

#include <array>
#include <functional>

#include <sys/types.h>

using ExtensionData = float;      
using Brightness = float;               
using ButtonIndex = int;                   

using DoneCallback = std::function<void()>;
using ExtensionCallback = std::function<void(std::array<ExtensionData, 4>)>;
using SingleButtonCallback = std::function<void(ButtonIndex)>;   
using AllButtonsCallback = std::function<void(void)>;               
using GpioCallback = std::function<void(void)>;                  


#endif

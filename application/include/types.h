#ifndef CALLBACK_TYPES_H_
#define CALLBACK_TYPES_H_

#include <array>
#include <functional>

#include <sys/types.h>

using ButtonIndex = int;                   

using DoneCallback = std::function<void()>;
using SingleButtonCallback = std::function<void(ButtonIndex)>;   
using AllButtonsCallback = std::function<void(void)>;               
using GpioCallback = std::function<void(void)>;                  


#endif

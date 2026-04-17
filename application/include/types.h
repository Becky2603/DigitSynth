#ifndef CALLBACK_TYPES_H_
#define CALLBACK_TYPES_H_

#include <functional>

#include <sys/types.h>


using GpioCallback = std::function<void(void)>;                  


#endif

#ifndef CALLBACK_TYPES_H_
#define CALLBACK_TYPES_H_


#include <array>
#include <cstdint>
#include <functional>
#include <optional>

#include <sys/types.h>
#include <tuple>

typedef std::optional<uint32_t>     AdcData;
typedef float                       ExtensionData;
typedef uint32_t ButtonData;

typedef uint32_t Frequency;
typedef float    FingerExtension; 

typedef std::function<void(AdcData)>       AdcCallback;
typedef std::function<void(std::array<ExtensionData, 4>)> ExtensionCallback;
typedef std::function<void(ButtonData)>    ButtonCallback;
typedef std::function<void(ssize_t)>       SpiCallback;
typedef std::function<void(void)>          GpioCallback;


#endif

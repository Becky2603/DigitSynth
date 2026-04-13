#ifndef CALLBACK_TYPES_H_
#define CALLBACK_TYPES_H_


#include <array>
#include <cstdint>
#include <functional>
#include <optional>

#include <sys/types.h>

typedef std::optional<uint32_t> AdcData;
typedef float                   ExtensionData;
typedef float                   Brightness;
typedef std::optional<bool>     ButtonData;

typedef uint32_t Frequency;

typedef std::function<void(AdcData)>       AdcCallback;
typedef std::function<void(std::array<ExtensionData, 4>)> ExtensionCallback;
typedef std::function<void(ButtonData)>    ButtonCallback;
typedef std::function<void(ssize_t)>       SpiCallback;
typedef std::function<void(void)>          GpioCallback;


#endif

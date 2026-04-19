#ifndef CALLBACK_TYPES_H_
#define CALLBACK_TYPES_H_


#include <array>
#include <cstdint>
#include <functional>
#include <optional>
#include "MidiTypes.hpp"

#include <sys/types.h>

typedef std::optional<uint32_t> AdcData;
typedef float                   ExtensionData;
typedef float                   Brightness;
typedef int                     ButtonIndex;

typedef std::function<void(std::array<ExtensionData, 4>)> ExtensionCallback;
typedef std::function<void(void)>                         GpioCallback;
typedef std::function<void(midi_message)>                 MidiCallback;


#endif

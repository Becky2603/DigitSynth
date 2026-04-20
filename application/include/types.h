#ifndef CALLBACK_TYPES_H_
#define CALLBACK_TYPES_H_


#include <array>
#include <functional>
#include "MidiTypes.hpp"

#include <sys/types.h>

typedef float                   ExtensionData;
typedef float                   Brightness;

typedef std::function<void(std::array<ExtensionData, 4>)> ExtensionCallback;
typedef std::function<void(void)>                         GpioCallback;
typedef std::function<void(midi_message)>                 MidiCallback;


#endif

#ifndef CALLBACK_TYPES_H_
#define CALLBACK_TYPES_H_


#include <cstdint>
#include <functional>
#include <optional>

typedef std::optional<uint32_t> AdcData;
typedef std::optional<float>    ExtensionData;
typedef std::optional<bool>     ButtonData;

typedef std::function<void(AdcData)>       AdcCallback;
typedef std::function<void(ExtensionData)> ExtensionCallback;
typedef std::function<void(ButtonData)>    ButtonCallback;



#endif

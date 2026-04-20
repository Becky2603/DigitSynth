#ifndef FLEX_DSP
#define FLEX_DSP

#include <array>
#include <functional>
#include "flex-sensor.hpp"
#include <Iir.h>
#include <memory>

using FilteredCallback = std::function<void(std::array<flex_sensor::ExtensionData, 4>)>;

class FlexDSP {
public:

    FlexDSP(std::unique_ptr<flex_sensor::IFlexSensor> flexSensor, float sampleRate = 100, float cutoffHz = 20.0f);

    // called by SynthController 
    void registerCallback(FilteredCallback callback);

    // reset filter state 
    void reset();

private:
    // one 4th order Butterworth low pass per sensor channel
    std::array<Iir::Butterworth::LowPass<4>, 4> filters;
    std::optional<FilteredCallback> callback;
    std::unique_ptr<flex_sensor::IFlexSensor> flexSensor; 
};

#endif

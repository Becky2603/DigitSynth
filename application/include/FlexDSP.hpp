#pragma once

#include "types.h"
#include <array>
#include <functional>
#include <optional>

using FilteredCallback = std::function<void(std::array<ExtensionData, 4>)>;

class FlexDSP {
public:
    // alpha controls cutoff lower = more smoothing, higher = more responsive
    // alpha = dt / (RC + dt)  where RC = 1 / (2 * pi * f_cutoff)
   
    explicit FlexDSP(float alpha = 0.15f);

    // called by FlexSensor, drop in place of SynthController's direct callback
    void onFlexData(std::array<ExtensionData, 4> rawData);

    // called by SynthController 
    void registerCallback(FilteredCallback callback);

    //runtime adjust cutoff without recompiling
    void setAlpha(float alpha);
    float getAlpha() const;

    // reset filter state 
    void reset();

private:
    float alpha;                          
    std::array<float, 4> state;            // y[n-1] for each channel
    bool initialised;                       // seed first sample directly

    std::optional<FilteredCallback> callback;

    float tick(int channel, float input);
};

#include "FlexDSP.hpp"

FlexDSP::FlexDSP(float alpha)
    : alpha(alpha)
    , state({0.0f, 0.0f, 0.0f, 0.0f})
    , initialised(false)
{}

// IIR core

float FlexDSP::tick(int channel, float input) {
    state[channel] = alpha * input + (1.0f - alpha) * state[channel];
    return state[channel];
}

void FlexDSP::onFlexData(std::array<ExtensionData, 4> rawData) {
    if (!callback.has_value()) { return; }

    if (!initialised) {
        state[0] = rawData[0];   // seed all 4 channels 
        state[1] = rawData[1];
        state[2] = rawData[2];
        state[3] = rawData[3];
        initialised = true;
        callback.value()(rawData);
        return;
    }

    std::array<ExtensionData, 4> filtered;
    for (int i = 0; i < 4; i++) {
        filtered[i] = tick(i, rawData[i]);
    }

    callback.value()(filtered);
}

    std::array<ExtensionData, 4> filtered;
    for (int i = 0; i < 4; i++) {
        filtered[i] = tick(i, rawData[i]);
    }

    callback.value()(filtered);
}

// config

void FlexDSP::registerCallback(FilteredCallback cb) {
    callback = cb;
}

void FlexDSP::setAlpha(float a) {
    // clamp to (0, 1) 
    if (a <= 0.0f) a = 0.01f;
    if (a >= 1.0f) a = 0.99f;
    alpha = a;
}

float FlexDSP::getAlpha() const {
    return alpha;
}

void FlexDSP::reset() {
    state.fill(0.0f);
    initialised = false;
}

#include "FlexDSP.hpp"

FlexDSP::FlexDSP(float sampleRate, float cutoffHz) {
    // iir1 setup() takes order + normalised cutoff (0.0–0.5)
    // where 0.5 = Nyquist. cutoffHz / sampleRate 
    double normalisedCutoff = cutoffHz / sampleRate;
    for (auto& f : filters) {
        f.setup(4, normalisedCutoff);
    }
}

void FlexDSP::onFlexData(std::array<ExtensionData, 4> rawData) {
    if (!callback.has_value()) { return; }

   std::array<ExtensionData, 4> filtered;
    for (int i = 0; i < 4; i++) {
        filtered[i] = (float)filters[i].filter(rawData[i]);
    }
    callback.value()(filtered);
}

// config

void FlexDSP::registerCallback(FilteredCallback cb) {
    callback = cb;
}

void FlexDSP::reset() {
    for (auto& f : filters) {
        f.reset();
    }
}

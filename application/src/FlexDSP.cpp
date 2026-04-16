#include "FlexDSP.hpp"

FlexDSP::FlexDSP(FlexSensor& sensor, float sampleRate, float cutoffHz) {
    double normalisedCutoff = cutoffHz / sampleRate;
    for (auto& f : filters) {
        f.setup(4, normalisedCutoff);
    }
    sensor.registerCallback([this](std::array<ExtensionData, 4> raw) {
        if (!this->callback.has_value()) { return; }
        std::array<ExtensionData, 4> filtered;
        for (int i = 0; i < 4; i++) {
            filtered[i] = static_cast<float>(this->filters[i].filter(raw[i]));
        }
        this->callback.value()(filtered);
    });
}

// config

void FlexDSP::registerCallback(FilteredCallback callback) {
    this->callback = callback;
}

void FlexDSP::reset() {
    for (auto& f : filters) {
        f.reset();
    }
}

#include "FlexDSP.hpp"
#include "flex-sensor.h"
#include <memory>

FlexDSP::FlexDSP(std::unique_ptr<flex_sensor::IFlexSensor> flexSensor, float sampleRate, float cutoffHz) :
    flexSensor(std::move(flexSensor)) 
{
    double normalisedCutoff = cutoffHz / sampleRate;
    for (auto& f : filters) {
        f.setup(4, normalisedCutoff);
    }
    this->flexSensor->registerCallback([this](std::array<flex_sensor::ExtensionData, 4> raw) {
        if (!this->callback.has_value()) { return; }
        std::array<flex_sensor::ExtensionData, 4> filtered;
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

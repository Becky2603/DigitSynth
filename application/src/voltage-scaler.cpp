#include "voltage-scaler.h"

using namespace voltage_scaler;

VoltageScaler::VoltageScaler() {}

float VoltageScaler::scale(float v, ADS1115settings::Input channel) {
    if (v > this->maxes[channel]) { this->maxes[channel] = v; }
    if (v < this->mins[channel])  { this->mins[channel] =  v; }
    
    float scaled = (v - this->mins[channel]) / (this->maxes[channel] - this->mins[channel]);
   
    if (channel == 0) {
        std::cout << this->maxes[channel] << " " << this->mins[channel] << " " << v << " " << scaled << "\n";
    }
    
    return scaled; 
}

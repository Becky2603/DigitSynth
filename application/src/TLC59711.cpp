#include "TLC59711.h"
#include <lgpio.h>
#include <stdexcept>

TLC59711::TLC59711(int data_pin, int clk_pin, int num_drivers)
    : _data_pin(data_pin),
      _clk_pin(clk_pin),
      _num_drivers(num_drivers),
      _pwm(num_drivers * CHANNELS_PER_DRIVER, 0)
{
    _gpio_handle = lgGpiochipOpen(0);
    if (_gpio_handle < 0)
        throw std::runtime_error("TLC59711: cannot open gpiochip0");

    if (lgGpioClaimOutput(_gpio_handle, 0, _data_pin, 0) < 0)
        throw std::runtime_error("TLC59711: cannot claim data pin");

    if (lgGpioClaimOutput(_gpio_handle, 0, _clk_pin, 0) < 0)
        throw std::runtime_error("TLC59711: cannot claim clock pin");
}

TLC59711::~TLC59711() {
    if (_gpio_handle >= 0) {
        lgGpioFree(_gpio_handle, _data_pin);
        lgGpioFree(_gpio_handle, _clk_pin);
        lgGpiochipClose(_gpio_handle);
    }
}

void TLC59711::setBrightness(uint8_t brightness) {
    _brightness = brightness;
}

// stubs — next commits
void TLC59711::start() {}
void TLC59711::stop()  {}
void TLC59711::update(const LEDFrame&) {}
void TLC59711::worker() {}
void TLC59711::buildPacket(std::vector<uint8_t>&) {}
void TLC59711::shiftOut(const std::vector<uint8_t>&) {}

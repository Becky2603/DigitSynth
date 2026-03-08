#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include <array>
#include <cstdint>
#include <functional>

class LEDDriver
{
public:
    static constexpr int NUM_CHANNELS = 12;

    using ErrorCallback = std::function<void(const char* error)>;

    LEDDriver();

    void init();
    void setLED(int ledIndex, int brightness);      // single channel, 0-100
    void update(const std::array<int, NUM_CHANNELS>& brightnessValues);
    void allOn();
    void allOff();
    void cleanup();

    void registerErrorCallback(ErrorCallback cb);

private:
    int spi_fd;
    std::array<uint16_t, NUM_CHANNELS> ledValues;
    ErrorCallback errorCallback;

    uint16_t normalise(int value);  // 0-100 → 16-bit PWM
    void writeSPI();                // packages and sends TLC59711 frame
};

#endif
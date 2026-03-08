#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include <vector>
#include <cstdint> // for uint16_t

class LEDDriver
{
private:
    int numChips; // Number of TLC59711 chips daisy-chained
    std::vector<uint16_t> ledValues; // store 16-bit values for each LED

    // Convert normalized brightness (0-100) to 16-bit PWM value
    uint16_t normalize(int value);

public:
    // Constructor
    // numChips: number of TLC59711 chips in chain
    LEDDriver(int numChips = 1);

    // Initialise SPI and TLC59711
    void init();

    // Set individual LED brightness using normalized value (0-100)
    void setLED(int ledIndex, int brightness);

    // Update LEDs: takes 10 normalized values (0-100)
    void update(const std::vector<int>& brightnessValues);

    // Turn all LEDs on at full brightness
    void allOn();

    // Turn all LEDs off
    void allOff();

    // Cleanup SPI
    void cleanup();
};

#endif
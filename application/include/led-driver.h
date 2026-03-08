#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include <vector>

class LEDDriver
{
private:
    std::vector<int> pins;

public:
    // Constructor
    LEDDriver(const std::vector<int>& ledPins);

    // Setup GPIO
    void init();

    // Control individual LEDs
    void turnOn(int index);
    void turnOff(int index);
    void toggle(int index);

    // Control all LEDs
    void allOn();
    void allOff();

    // Cleanup
    void cleanup();
};

#endif
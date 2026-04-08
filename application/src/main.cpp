#include <array>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <gpiod.hpp>
#include "button-driver.h"
#include "flex-sensor.h"
#include "foo.h"
#include "gpio.h"
#include "types.h"
#include <linux/spi/spi.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <ads1115rpi.h>
#include <TLC59711.h>

int main(int argc, char **argv) {
    (void) argc;
    (void) argv; 
    
    gpio::setupGpio();

    ButtonDriver bd;
    TLC59711 leds(17, 27);
    FlexSensor flexSensor;
    TLC59711::Channels channels;
    
    flexSensor.registerCallback([&channels, &leds] (std::array<ExtensionData, 4> data) {
        channels[0] = data[0];
        leds.update(channels);
    });
    
    flexSensor.begin();
    getchar();
    
    return 0;
}

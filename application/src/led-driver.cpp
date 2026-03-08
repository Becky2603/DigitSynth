#include "include/led-driver.h"

#include <fcntl.h>      // open()
#include <unistd.h>     // close()
#include <sys/ioctl.h>  // ioctl()
#include <linux/spi/spidev.h>
#include <cstring>      // memset()
#include <stdexcept>    // std::runtime_error

// TLC59711 constants
static constexpr uint32_t TLC_CMD   = 0x25000000; //hard codded command for command bit and control bits 
static constexpr uint32_t SPI_SPEED = 1000000;    // 1 MHz SPI speed setter
static constexpr uint8_t  SPI_MODE  = 0;          // mode 0: CPOL=0, CPHA=0
static constexpr int      FRAME_SIZE = 28;        // 4 bytes command + 24 bytes (12 x 16-bit)


LEDDriver::LEDDriver() : spi_fd(-1) {
    ledValues.fill(0);
}


void LEDDriver::init() {
}

// cleanup: close SPI file descriptor
void LEDDriver::cleanup() {
    if (spi_fd >= 0) {
        close(spi_fd);
        spi_fd = -1;
    }
}

// normalise: convert 0-1 brightness to 16-bit PWM value (0-65535)
uint16_t LEDDriver::normalise(int value) {
}


// writeSPI: pack TLC59711 frame and send over SPI
void LEDDriver::writeSPI() {
}


// update: receive 12 normalised brightness values and write to hardware
void LEDDriver::update(const std::array<int, NUM_CHANNELS>& brightnessValues) {
}



// setLED: set a single channel brightness and write to hardware
void LEDDriver::setLED(int ledIndex, int brightness) {
}

// allOn: set all channels to full brightness
void LEDDriver::allOn() {
}

// allOff: turn all channels off
void LEDDriver::allOff() {
}

// registerErrorCallback: store error callback functor
void LEDDriver::registerErrorCallback(ErrorCallback cb) {
}
#include "TLC59711.h"
#include <lgpio.h>
#include <stdexcept>
#include <cmath>
#include <unistd.h>
#include <iostream>

constexpr int TLC59711::FRAME_TO_GS[10];

TLC59711::TLC59711(int data_pin, int clk_pin, int num_drivers)
    : _data_pin(data_pin),
      _clk_pin(clk_pin),
      _num_drivers(num_drivers),
      _pwm(num_drivers * CHANNELS_PER_DRIVER, 0)
{
    // Open chip only — pins are claimed in worker() on the worker thread
    _gpio_handle = lgGpiochipOpen(0);
    if (_gpio_handle < 0)
        throw std::runtime_error("TLC59711: cannot open gpiochip0");
}

TLC59711::~TLC59711() {
    stop();
    if (_gpio_handle >= 0)
        lgGpiochipClose(_gpio_handle);
}

void TLC59711::start() {
    if (_running) return;

    if (pipe(_pipe_fd) < 0)
        throw std::runtime_error("TLC59711: pipe() failed");

    _running = true;
    _thread  = std::thread(&TLC59711::worker, this);
}

void TLC59711::stop() {
    if (!_running) return;
    _running = false;

    // Closing write end unblocks worker's read() with EOF
    if (_pipe_fd[1] >= 0) {
        close(_pipe_fd[1]);
        _pipe_fd[1] = -1;
    }

    if (_thread.joinable())
        _thread.join();

    if (_pipe_fd[0] >= 0) {
        close(_pipe_fd[0]);
        _pipe_fd[0] = -1;
    }
}

void TLC59711::update(const LEDFrame& frame) {
    if (_pipe_fd[1] < 0) return;
    const ssize_t n = ::write(_pipe_fd[1], &frame, sizeof(LEDFrame));
    if (n < 0)
        std::cerr << "TLC59711::update: pipe write failed\n";
}

void TLC59711::setBrightness(uint8_t brightness) {
    _brightness = brightness;
}

void TLC59711::worker() {
    // Claim pins on this thread — lgpio ties GPIO access to the claiming thread
    if (lgGpioClaimOutput(_gpio_handle, 0, _data_pin, 0) < 0) {
        std::cerr << "TLC59711: worker cannot claim data pin\n";
        return;
    }
    if (lgGpioClaimOutput(_gpio_handle, 0, _clk_pin, 0) < 0) {
        std::cerr << "TLC59711: worker cannot claim clock pin\n";
        lgGpioFree(_gpio_handle, _data_pin);
        return;
    }

    LEDFrame frame{};

    while (_running) {
        // Blocking read — thread sleeps here until update() writes a frame
        // or stop() closes the write end (EOF)
        const ssize_t n = ::read(_pipe_fd[0], &frame, sizeof(LEDFrame));

        if (n <= 0) break;
        if (n != sizeof(LEDFrame)) continue;

        std::fill(_pwm.begin(), _pwm.end(), 0);
        for (int i = 0; i < NUM_LEDS; ++i)
            _pwm[FRAME_TO_GS[i]] = toGS(frame.channels[i]);

        std::vector<uint8_t> buf;
        buf.reserve(static_cast<size_t>(_num_drivers) * 28);
        buildPacket(buf);
        shiftOut(buf);
    }

    // Release pins before thread exits
    lgGpioWrite(_gpio_handle, _data_pin, 0);
    lgGpioWrite(_gpio_handle, _clk_pin,  0);
    lgGpioFree(_gpio_handle, _data_pin);
    lgGpioFree(_gpio_handle, _clk_pin);
}

void TLC59711::buildPacket(std::vector<uint8_t>& buf) const {
    for (int d = 0; d < _num_drivers; ++d) {
        const uint16_t* gs = _pwm.data() + d * CHANNELS_PER_DRIVER;

        const uint8_t bc = _brightness & 0x7Fu;
        uint32_t cmd = 0;
        cmd |= uint32_t{0x25} << 26;
        cmd |= uint32_t{1}    << 25;   // OUTTMG
        cmd |= uint32_t{0}    << 24;   // EXTGCK
        cmd |= uint32_t{1}    << 23;   // TMGRST
        cmd |= uint32_t{1}    << 22;   // DSPRPT
        cmd |= uint32_t{0}    << 21;   // BLANK
        cmd |= uint32_t{bc}   << 14;   // BCB
        cmd |= uint32_t{bc}   <<  7;   // BCG
        cmd |= uint32_t{bc};           // BCR

        buf.push_back(static_cast<uint8_t>(cmd >> 24));
        buf.push_back(static_cast<uint8_t>(cmd >> 16));
        buf.push_back(static_cast<uint8_t>(cmd >>  8));
        buf.push_back(static_cast<uint8_t>(cmd      ));

        for (int led = 3; led >= 0; --led) {
            for (int ch = 2; ch >= 0; --ch) {
                const uint16_t v = gs[led * 3 + ch];
                buf.push_back(static_cast<uint8_t>(v >> 8));
                buf.push_back(static_cast<uint8_t>(v     ));
            }
        }
    }
}

void TLC59711::shiftOut(const std::vector<uint8_t>& buf) const {
    struct timespec ts{ 0, HALF_PERIOD_NS };

    for (uint8_t byte : buf) {
        for (int bit = 7; bit >= 0; --bit) {
            lgGpioWrite(_gpio_handle, _data_pin, (byte >> bit) & 1);
            nanosleep(&ts, nullptr);
            lgGpioWrite(_gpio_handle, _clk_pin, 1);
            nanosleep(&ts, nullptr);
            lgGpioWrite(_gpio_handle, _clk_pin, 0);
        }
    }
    lgGpioWrite(_gpio_handle, _data_pin, 0);
    ts.tv_nsec = HALF_PERIOD_NS * 20;
    nanosleep(&ts, nullptr);
}

uint16_t TLC59711::toGS(Brightness b) {
    if (b <= 0.0f) return 0;
    if (b >= 1.0f) return 0xFFFF;
    return static_cast<uint16_t>(b * 65535.0f + 0.5f);
}
#include "TLC59711.h"
#include <lgpio.h>
#include <stdexcept>
#include <cmath>
#include <unistd.h>
#include <cstring>
#include <iostream>

// ── Channel map definition ───────────────────────────────────────────────────
constexpr int TLC59711::FRAME_TO_GS[10];

// ── Construction / destruction ───────────────────────────────────────────────

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
    stop();   // safe to call if never started — checks _running
    if (_gpio_handle >= 0) {
        lgGpioWrite(_gpio_handle, _data_pin, 0);
        lgGpioWrite(_gpio_handle, _clk_pin,  0);
        lgGpioFree(_gpio_handle, _data_pin);
        lgGpioFree(_gpio_handle, _clk_pin);
        lgGpiochipClose(_gpio_handle);
    }
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

    // Unblock the worker's read() by closing the write end of the pipe.
    // The worker sees EOF (read returns 0) and exits its loop cleanly.
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

// Called from any thread. Writes the frame into the pipe and returns
// The worker owns all downstream processing.

void TLC59711::update(const LEDFrame& frame) {
    if (_pipe_fd[1] < 0) return;
    // A single write() of ≤PIPE_BUF bytes is atomic on POSIX.
    // sizeof(LEDFrame) is well under PIPE_BUF (4096 bytes minimum).
    const ssize_t n = write(_pipe_fd[1], &frame, sizeof(LEDFrame));
    if (n < 0)
        std::cerr << "TLC59711::update: pipe write failed\n";
}

void TLC59711::setBrightness(uint8_t brightness) {
    _brightness = brightness;
}

// Runs in its own thread. Blocks on pipe read — woken by update().
// Owns _pwm exclusively; no locking needed.

void TLC59711::worker() {
    LEDFrame frame{};

    while (_running) {
        // thread sleeps here until update() writes a frame
        // or stop() closes the write end (returning 0 = EOF).
        const ssize_t n = read(_pipe_fd[0], &frame, sizeof(LEDFrame));

        if (n <= 0) break;   // EOF stop() was called
        if (n != sizeof(LEDFrame)) continue;  // ignore malformed frames

        // Convert normalised Brightness values to 16-bit GS
        std::fill(_pwm.begin(), _pwm.end(), 0);
        for (int i = 0; i < NUM_LEDS; ++i)
            _pwm[FRAME_TO_GS[i]] = toGS(frame.channels[i]);

        // Assemble and transmit
        std::vector<uint8_t> buf;
        buf.reserve(static_cast<size_t>(_num_drivers) * 28);
        buildPacket(buf);
        shiftOut(buf);
    }
}


void TLC59711::buildPacket(std::vector<uint8_t>& buf) const {
    for (int d = 0; d < _num_drivers; ++d) {
        const uint16_t* gs = _pwm.data() + d * CHANNELS_PER_DRIVER;

        // 32-bit command word
        const uint8_t bc = _brightness & 0x7Fu;
        uint32_t cmd = 0;
        cmd |= uint32_t{0x25} << 26;   // WRCMD
        cmd |= uint32_t{1}    << 25;   // OUTTMG  — rising edge
        cmd |= uint32_t{0}    << 24;   // EXTGCK  — internal osc
        cmd |= uint32_t{1}    << 23;   // TMGRST  — reset display timing
        cmd |= uint32_t{1}    << 22;   // DSPRPT  — auto-repeat
        cmd |= uint32_t{0}    << 21;   // BLANK   — outputs on
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
    // CLK idles low for >> 8 clock periods
    lgGpioWrite(_gpio_handle, _data_pin, 0);
    ts.tv_nsec = HALF_PERIOD_NS * 20;
    nanosleep(&ts, nullptr);
}


uint16_t TLC59711::toGS(Brightness b) {
    if (b <= 0.0f) return 0;
    if (b >= 1.0f) return 0xFFFF;
    return static_cast<uint16_t>(std::pow(b, 2.2f) * 65535.0f + 0.5f);
}
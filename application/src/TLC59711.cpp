#include "TLC59711.h"
#include <gpiod.hpp>
#include <stdexcept>
#include <cmath>
#include <iostream>

constexpr int TLC59711::FRAME_TO_GS[10];

TLC59711::TLC59711(int data_pin, int clk_pin, int num_drivers)
    : _data_pin(data_pin),
      _clk_pin(clk_pin),
      _num_drivers(num_drivers),
      _pwm(num_drivers * CHANNELS_PER_DRIVER, 0)
{
    // No GPIO resources to acquire at construction time —
    // the worker thread opens and owns the line_request for its lifetime.
}

TLC59711::~TLC59711() {
    stop();
}

void TLC59711::start() {
    if (_running) return;
    _running = true;
    _thread  = std::thread(&TLC59711::worker, this);
}

void TLC59711::stop() {
    if (!_running) return;

    {
        std::lock_guard<std::mutex> lock(_mutex);
        _running = false;
    }
    _cv.notify_one();   // wake worker so it can see _running == false

    if (_thread.joinable())
        _thread.join();
}

void TLC59711::update(const Channels& channels) {
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _pending = channels;
        _dirty   = true;
    }
    _cv.notify_one();
}

void TLC59711::setBrightness(uint8_t brightness) {
    _brightness = brightness;
}

void TLC59711::worker() {
    // Open chip and claim both output lines — held for the worker's lifetime.
    gpiod::chip chip("/dev/gpiochip0");

    gpiod::line_config line_config;
    line_config.add_line_settings(
        { static_cast<unsigned int>(_data_pin), static_cast<unsigned int>(_clk_pin) },
        gpiod::line_settings()
            .set_direction(gpiod::line::direction::OUTPUT)
            .set_output_value(gpiod::line::value::INACTIVE)
    );

    gpiod::line_request request = chip
        .prepare_request()
        .set_consumer("tlc59711")
        .set_line_config(line_config)
        .do_request();

    Channels channels{};

    while (true) {
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _cv.wait(lock, [this] { return _dirty || !_running; });

            if (!_running) break;

            channels = _pending;
            _dirty   = false;
        }
        // Lock is released here — calling thread is free immediately.

        std::fill(_pwm.begin(), _pwm.end(), 0);
        for (int i = 0; i < NUM_LEDS; ++i)
            _pwm[FRAME_TO_GS[i]] = toGS(channels[i]);

        std::vector<uint8_t> buf;
        buf.reserve(static_cast<size_t>(_num_drivers) * 28);
        buildPacket(buf);
        shiftOut(buf, request);
    }

    // Drive both pins low before releasing.
    request.set_value(_data_pin, gpiod::line::value::INACTIVE);
    request.set_value(_clk_pin,  gpiod::line::value::INACTIVE);
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

void TLC59711::shiftOut(const std::vector<uint8_t>& buf,
                        gpiod::line_request& request) const {
    struct timespec ts{ 0, HALF_PERIOD_NS };

    for (uint8_t byte : buf) {
        for (int bit = 7; bit >= 0; --bit) {
            const auto data_val = ((byte >> bit) & 1)
                ? gpiod::line::value::ACTIVE
                : gpiod::line::value::INACTIVE;

            request.set_value(_data_pin, data_val);
            nanosleep(&ts, nullptr);
            request.set_value(_clk_pin, gpiod::line::value::ACTIVE);
            nanosleep(&ts, nullptr);
            request.set_value(_clk_pin, gpiod::line::value::INACTIVE);
        }
    }

    request.set_value(_data_pin, gpiod::line::value::INACTIVE);
    ts.tv_nsec = HALF_PERIOD_NS * 20;
    nanosleep(&ts, nullptr);
}

uint16_t TLC59711::toGS(Brightness b) {
    if (b <= 0.0f) return 0;
    if (b >= 1.0f) return 0xFFFF;
    return static_cast<uint16_t>(b * 65535.0f + 0.5f);
}
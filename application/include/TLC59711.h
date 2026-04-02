#pragma once
#include <cstdint>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <gpiod.hpp>
#include "types.h"

struct LEDFrame {
    Brightness channels[10];
};

class TLC59711 {
    friend class TLC59711Test;
public:
    TLC59711(int data_pin, int clk_pin, int num_drivers = 1);
    ~TLC59711();

    void start();
    void stop();

    // Non-blocking — copies frame into shared buffer and wakes worker thread.
    void update(const LEDFrame& frame);

    void setBrightness(uint8_t brightness);

private:
    // Blocking I/O loop — woken by update(), owns all GPIO access.
    void worker();

    void buildPacket(std::vector<uint8_t>& buf) const;
    void shiftOut(const std::vector<uint8_t>& buf,
                  gpiod::line_request& request) const;
    static uint16_t toGS(Brightness b);

    static constexpr int FRAME_TO_GS[10] = {
        0, 1, 2,
        3, 4, 5,
        6, 7, 8,
        9
    };

    static constexpr int  CHANNELS_PER_DRIVER = 12;
    static constexpr int  NUM_LEDS            = 10;
    static constexpr long HALF_PERIOD_NS      = 5000L;

    std::thread              _thread;
    std::atomic<bool>        _running{false};

    // Shared state between calling thread and worker thread.
    std::mutex               _mutex;
    std::condition_variable  _cv;
    LEDFrame                 _pendingFrame{};
    bool                     _dirty{false};

    int      _data_pin{0};
    int      _clk_pin{0};
    int      _num_drivers{1};

    std::atomic<uint8_t> _brightness{127};

    // Owned exclusively by worker thread — no mutex needed.
    std::vector<uint16_t> _pwm;
};
#pragma once
#include <cstdint>
#include <vector>
#include <thread>
#include <atomic>
#include "types.h"

struct LEDFrame {
    Brightness channels[10];   // 0.0–1.0
};

class TLC59711 {
public:
    TLC59711(int data_pin, int clk_pin, int num_drivers = 1);
    ~TLC59711();

    void start();
    void stop();

    void update(const LEDFrame& frame);
    void setBrightness(uint8_t brightness);  // global hardware scaler 0–127

private:
    void worker();
    void buildPacket(std::vector<uint8_t>& buf);
    void shiftOut(const std::vector<uint8_t>& buf);

    static constexpr int CHANNELS_PER_DRIVER = 12;
    static constexpr int NUM_LEDS            = 10;

    std::thread       _thread;
    std::atomic<bool> _running{false};

    int _gpio_handle{-1};
    int _data_pin{0};
    int _clk_pin{0};
    int _num_drivers{1};

    int _pipe_fd[2]{-1, -1};  // [0]=read (worker), [1]=write (update)

    uint8_t _brightness{127};

    std::vector<uint16_t> _pwm;  // only touched by worker — no mutex needed
};
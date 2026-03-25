#include "patterns.h"
#include <cmath>
#include <chrono>
#include <thread>

void pattern_ripple(TLC59711& tlc) {
    static constexpr float TWO_PI      = 6.28318f;
    static constexpr int   N_FINGERS   = 10;
    static constexpr float SPEED       = 2;
    static constexpr int   SLEEP_TIME_MS = 5;

    // Spread fingers over 1/3 of the cycle so ~3 are bright at once
    const float phase_step = (TWO_PI / 3.0f) / N_FINGERS;

    const auto t_start = std::chrono::steady_clock::now();

    while (true) {
        const float t_secs = std::chrono::duration<float>(
            std::chrono::steady_clock::now() - t_start).count();

        LEDFrame frame{};
        for (int f = 0; f < N_FINGERS; ++f) {
            const float raw = std::sin(TWO_PI * SPEED * t_secs + f * phase_step);
            frame.channels[f] = (raw + 1.0f) / 2.0f;
        }

        tlc.update(frame);
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME_MS));
    }
}
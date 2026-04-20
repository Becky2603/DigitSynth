#include "patterns.hpp"
#include <cmath>
#include <stdexcept>
#include <sys/timerfd.h>
#include <unistd.h>

/**
 * Creates a timerfd that fires at the given interval in milliseconds.
 * Returns the file descriptor (caller must close it).
 */
static int makeTimerFd(long period_ms) {
    int fd = timerfd_create(CLOCK_MONOTONIC, 0);
    if (fd < 0)
        throw std::runtime_error("timerfd_create failed");

    struct itimerspec its{};
    its.it_value.tv_sec     = period_ms / 1000;
    its.it_value.tv_nsec    = (period_ms % 1000) * 1000000L;
    its.it_interval.tv_sec  = its.it_value.tv_sec;
    its.it_interval.tv_nsec = its.it_value.tv_nsec;

    if (timerfd_settime(fd, 0, &its, nullptr) < 0) {
        close(fd);
        throw std::runtime_error("timerfd_settime failed");
    }
    return fd;
}

/**
 * Block until the next timer tick
 * Returns false if _running has been cleared
 *@param fd  file descriptor of the timerfd to wait on
 *@param running  reference to the atomic flag that indicates whether the pattern should keep running
 */
static bool waitTick(int fd, const std::atomic<bool>& running) {
    uint64_t exp = 0;
    if (read(fd, &exp, sizeof(exp)) < 0)
        return false;
    return running.load();
}

/**
    * IPattern::start starts the pattern in a background thread, which runs the run() method. 
 */
void led_pattern::IPattern::start(DoneCallback onDone) {
    if(this->_running){
        return;
    }
    _onDone  = std::move(onDone);
    _running = true;
    // Each pattern runs in its own thread (Ch. 3.3.1).
    _thread  = std::thread([this]{ run(); });
}

void led_pattern::IPattern::stop() {
    // Set the flag so the worker exits its blocking loop (Ch. 3.3.3).
    _running = false;
    if (_thread.joinable())
        _thread.join();  // wait for clean exit (Ch. 3.3.2)
}

// ---------------------------------------------------------------------------
// PatternRipple
// ---------------------------------------------------------------------------

void led_pattern::PatternRipple::run() {
    static constexpr float TWO_PI    = 6.28318f;
    static constexpr int   N_FINGERS = 8;  // 8 LEDs
    static constexpr float SPEED     = 1.0f;
    static constexpr long  STEP_MS   = 40;

    // Physical channel indices in left-to-right visual order:
    //   L_pinky=5, L_ring=0, L_middle=4, L_index=1,
    //   R_index=2, R_middle=6, R_ring=3, R_pinky=7
    // Phase offset f=0 leads (leftmost), f=7 lags (rightmost).
    static constexpr int LEFT_TO_RIGHT[N_FINGERS] = { 5, 0, 4, 1, 2, 6, 3, 7 };

    int fd = makeTimerFd(STEP_MS);

    const float phase_step = (TWO_PI / 3.0f) / N_FINGERS;
    const auto  t_start    = std::chrono::steady_clock::now();

    // Loop runs until stop() sets _running = false (Ch. 3.3.3).
    while (_running) {
        const float t_secs = std::chrono::duration<float>(
            std::chrono::steady_clock::now() - t_start).count();

        led_driver::ILedDriver::Channels channels{};
        for (int f = 0; f < N_FINGERS; ++f) {
            const float raw = std::sin(TWO_PI * SPEED * t_secs + f * phase_step);
            channels[LEFT_TO_RIGHT[f]] = (raw + 1.0f) / 2.0f;
        }
        _tlc.update(channels);

        // Blocking timerfd read provides the frame timing (Ch. 3.4).
        if (!waitTick(fd, _running)) break;
    }

    close(fd);

    if (_onDone)
        _onDone();

    _running = false;
}
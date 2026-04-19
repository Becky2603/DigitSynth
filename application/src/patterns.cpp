#include "patterns.h"
#include <cmath>
#include <stdexcept>
#include <sys/timerfd.h>
#include <unistd.h>

// ---------------------------------------------------------------------------
// helpers
// ---------------------------------------------------------------------------

/**
 * Creates a timerfd that fires at the given interval in milliseconds.
 * Returns the file descriptor (caller must close it).
 * Follows course Ch. 3.4: reliable timing via blocking I/O on a timerfd,
 * not sleep_for / usleep.
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
 * Block until the next timer tick.
 * Returns false if _running has been cleared (caller should exit its loop).
 */
static bool waitTick(int fd, const std::atomic<bool>& running) {
    uint64_t exp = 0;
    // Blocking read — the course-approved way to create event timing (Ch. 3.4).
    if (read(fd, &exp, sizeof(exp)) < 0)
        return false;
    return running.load();
}

/**
 * Re-arm an existing timerfd with a new period.
 * Used to switch between the step rate and the hold duration mid-run.
 */
static void rearmTimerFd(int fd, long period_ms, bool repeat) {
    struct itimerspec its{};
    its.it_value.tv_sec     = period_ms / 1000;
    its.it_value.tv_nsec    = (period_ms % 1000) * 1000000L;
    if (repeat) {
        its.it_interval = its.it_value;
    }
    timerfd_settime(fd, 0, &its, nullptr);
}

// ---------------------------------------------------------------------------
// Pattern base
// ---------------------------------------------------------------------------

void Pattern::start(DoneCallback onDone) {
    if(this->_running){
        return;
    }
    _onDone  = std::move(onDone);
    _running = true;
    // Each pattern runs in its own thread (Ch. 3.3.1).
    _thread  = std::thread([this]{ run(); });
}

void Pattern::stop() {
    // Set the flag so the worker exits its blocking loop (Ch. 3.3.3).
    _running = false;
    if (_thread.joinable())
        _thread.join();  // wait for clean exit (Ch. 3.3.2)
}

// ---------------------------------------------------------------------------
// PatternFade
// ---------------------------------------------------------------------------

void PatternFade::run() {
    static constexpr long STEP_MS = 40;  
    static constexpr long HOLD_MS = 500;
    static constexpr int  N_STEPS = 20;

    int fd = makeTimerFd(STEP_MS);

    auto applyBrightness = [&](float b) {
        TLC59711::Channels ch{};
        for (auto& v : ch) v = b;
        _tlc.update(ch);
    };

    while (_running) {

        // --- Fade in ---
        for (int i = 0; i <= N_STEPS && _running; ++i) {
            applyBrightness(i / static_cast<float>(N_STEPS));
            if (!waitTick(fd, _running)) goto done;
        }

        // --- Hold ---
        if (_running) {
            rearmTimerFd(fd, HOLD_MS, /*repeat=*/false);
            if (!waitTick(fd, _running)) goto done;
            rearmTimerFd(fd, STEP_MS,  /*repeat=*/true);
        }

        // --- Fade out ---
        for (int i = N_STEPS; i >= 0 && _running; --i) {
            applyBrightness(i / static_cast<float>(N_STEPS));
            if (!waitTick(fd, _running)) goto done;
        }
    }

done:
    close(fd);

    if (_onDone)
        _onDone();

    _running = false;
}

// ---------------------------------------------------------------------------
// PatternRipple
// ---------------------------------------------------------------------------

void PatternRipple::run() {
    static constexpr float TWO_PI    = 6.28318f;
    static constexpr int   N_FINGERS = TLC59711::NUM_LEDS;  // 8 LEDs
    static constexpr float SPEED     = 1.0f;
    static constexpr long  STEP_MS   = 40;

    int fd = makeTimerFd(STEP_MS);

    const float phase_step = (TWO_PI / 3.0f) / N_FINGERS;
    const auto  t_start    = std::chrono::steady_clock::now();

    // Loop runs until stop() sets _running = false (Ch. 3.3.3).
    while (_running) {
        const float t_secs = std::chrono::duration<float>(
            std::chrono::steady_clock::now() - t_start).count();

        TLC59711::Channels channels{};
        for (int f = 0; f < N_FINGERS; ++f) {
            const float raw = std::sin(TWO_PI * SPEED * t_secs + f * phase_step);
            channels[f] = (raw + 1.0f) / 2.0f;
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

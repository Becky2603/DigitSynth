#pragma once

#include <functional>
#include <atomic>
#include <thread>
#include "ILedDriver.hpp"

namespace led_pattern {

/**
 * Minimal interface that LedController depends on — nothing else needed.
 */
class ILEDPattern {
public:
    using DoneCallback = std::function<void()>;
    virtual ~ILEDPattern() = default;
    virtual void start(DoneCallback onDone = nullptr) = 0;
    virtual void stop() = 0;
};

/**
 * Base class for LED patterns.
 *
 * Course guidance applied:
 *  - Each pattern runs in its own thread (Ch. 3) so it never blocks the caller.
 *  - Timing comes from a timerfd blocking read (Ch. 3.4), not sleep_for.
 *  - A std::atomic<bool> running flag lets the caller stop the pattern cleanly,
 *    and join() waits for the thread to finish (Ch. 3.3.2 / 3.3.3).
 *  - A std::function callback (Ch. 2.2.1) is fired when the pattern completes,
 *    so the caller can chain the next action without polling.
 */
class Pattern : public ILEDPattern {
public:
    virtual ~Pattern() { stop(); }

    /**
     * Start the pattern in a background thread.
     * @param onDone  Called (from the worker thread) when the pattern finishes
     *                naturally.  May be nullptr.
     */
    void start(DoneCallback onDone = nullptr) override;

    /**
     * Signal the pattern to stop and block until the thread exits.
     * Safe to call even if the pattern has already finished.
     */
    void stop() override;

protected:
    std::atomic<bool>  _running{false};
    DoneCallback       _onDone;

    /**
     * Implemented by each derived pattern.
     * Must honour _running: exit as soon as _running becomes false.
     * Timing must use blocking I/O (timerfd), not sleep_for.
     */
    virtual void run() = 0;

private:
    std::thread _thread;
};

/**
 * Ripples a sine wave across all fingers — runs until stop() is called.
 */
class PatternRipple : public Pattern {
public:
    explicit PatternRipple(led_driver::ILedDriver& tlc) : _tlc(tlc) {}

protected:
    void run() override;
private:
    led_driver::ILedDriver& _tlc;
};

} // namespace led_pattern

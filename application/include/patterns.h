#pragma once

#include <functional>
#include <atomic>
#include <thread>
#include "ILedDriver.hpp"

namespace led_pattern {


using DoneCallback = std::function<void()>;
/**
 * Base class for LED patterns.
 *
 * Each pattern runs in its own thread, which is started by calling start() and stopped by calling stop().
 */
class IPattern {
public:
    IPattern(led_driver::ILedDriver &) {};
    virtual ~IPattern() { stop(); }

    /**
     * Start the pattern in a background thread.
     * @param onDone  Called (from the worker thread) when the pattern finishes
     *                naturally.
     */
    virtual void start(DoneCallback onDone = nullptr);

    /**
     * Signal the pattern to stop and block until the thread exits.
     */
    virtual void stop();

protected:
    std::atomic<bool>  _running{false};
    DoneCallback       _onDone;

    /**
     * Implemented by each derived pattern.
     * Must honour _running: exit as soon as _running becomes false.
     */
    virtual void run() = 0;

private:
    std::thread _thread;
};

/**
 * Ripples a sine wave across all fingers — runs until stop() is called.
 */
class PatternRipple : public IPattern {
public:
    explicit PatternRipple(led_driver::ILedDriver& tlc) : IPattern(tlc), _tlc(tlc) {}

protected:
    void run() override;
private:
    led_driver::ILedDriver& _tlc;
};

} // namespace led_pattern

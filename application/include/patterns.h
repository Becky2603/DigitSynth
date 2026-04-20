#pragma once

#include <functional>
#include <atomic>
#include <thread>
#include "ILedDriver.hpp"

namespace led_pattern {

/**
 * @brief Callback invoked when a pattern completes naturally.
 *
 * this is executed from the worker thread .
 */
using DoneCallback = std::function<void()>;

/**
 * @class IPattern
 * @brief Abstract base class for asynchronous LED patterns.
 *
 * Each pattern runs in its own worker thread with the lifecycle being controlled via
 * start() and stop():
 *
 * - start() launches the pattern in a background thread.
 * - stop() begins termination and waits for the thread to exit.
 *
 * Derived classes must implement run() and periodically check `_running`
 * to terminate promptly when requested.
 *
 */
class IPattern {
public:
    /**
     * @brief Construct a pattern with access to an LED driver.
     *
     * @param driver Reference to the LED driver used for output.
     */
    explicit IPattern(led_driver::ILedDriver &driver) {}

    /**
     * @brief Destructor ensures the worker thread is stopped.
     */
    virtual ~IPattern() { stop(); }

    /**
     * @brief Start the pattern in a background thread.
     *
     * If the pattern is already running, calling start() has no effect.
     *
     * @param onDone Optional callback invoked when the pattern completes
     *               naturally.
     */
    virtual void start(DoneCallback onDone = nullptr);

    /**
     * @brief Stop the pattern and wait for the worker thread to exit.
     *
     * Safe to call multiple times. If the pattern is not running, this is a no-op.
     */
    virtual void stop();

protected:
    std::atomic<bool>  _running{false}; 
    DoneCallback       _onDone;         

    /**
     * @brief Pattern execution loop.
     * Derived classes must implement this method to define the pattern behavior. 
     */
    virtual void run() = 0;

private:
    std::thread _thread;
};

/**
 * @class PatternRipple
 * @brief Ripple pattern that uses a phase offset sine wave 
 *
 * This pattern animates a wave across all LED channels and runs until stop() is called.
 */
class PatternRipple : public IPattern {
public:
    /**
     * @brief Construct a ripple pattern.
     *
     * @param tlc Reference to the LED driver used to control outputs.
     */
    explicit PatternRipple(led_driver::ILedDriver& tlc)
        : IPattern(tlc), _tlc(tlc) {}

protected:
    /**
     * @brief Main ripple animation loop.
     *
     * Generates a sine-wave pattern across LEDs while `_running` is true.
     */
    void run() override;

private:
    led_driver::ILedDriver& _tlc;
};

} // namespace led_pattern
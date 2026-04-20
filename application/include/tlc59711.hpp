#include "i-led-driver.hpp"
#include <string>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <atomic>


namespace led_driver {
/**
 * @class TLC59711
 * @brief LED driver for the TLC59711 using Linux spidev (hardware SPI).
 *
 * This implementation provides asynchronous LED updates using a worker thread. 
 * Calls to update() are non-blocking and signal the worker
 * thread to transmit new PWM data over SPI.
 *
 *
 * @note This class is not copyable or movable due to thread and file descriptor ownership.
 */
class TLC59711 : public led_driver::ILedDriver {
    friend class TLC59711Test;

public:
    /**
     * @brief Constructs a TLC59711 driver.
     *
     * @param spi_device Path to the spidev device
     * @param spi_speed_hz SPI clock speed
     * @param num_drivers Number of daisy-chained TLC59711 devices in the chain (we only use one, but this allows for future expansion)
     */
    explicit TLC59711(std::string spi_device = "/dev/spidev0.0",
                      uint32_t    spi_speed_hz = 1'000'000,
                      int         num_drivers = 1);

    /**
     * @brief Destructor stops the worker thread and releases resources.
     */
    ~TLC59711();

    /**
     * @brief Start the driver.
     *
     * Opens the SPI device and launches the worker thread.
     */
    void start();

    /**
     * @brief Stop the driver.
     *
     * Signals the worker thread to exit, waits for it to join,
     * and closes the SPI device.
     *
     */
    void stop();

    /**
     * @brief Submits a new set of LED channel values.
     *
     * Non-blocking: copies the provided channel data into an internal buffer beofre returning immediately.
     * Signals the worker thread to perform an SPI transfer.
     *
     * @param channels Array of normalized brightness values from 0.0–1.0
     */
    void update(const Channels& channels);

    /**
     * @brief Setter for LED brightness scaling factor (0–255).
     *
     * Applied atomically and used during PWM conversion.
     *
     * @param brightness Brightness value (0–255).
     */
    void setBrightness(uint8_t brightness);

private:
    /**
     * @brief Worker thread loop.
     *
     * Waits for new data to be available through condition variable. 
     * Performs blocking SPI I/O.
     * Owns the SPI file descriptor exclusively.
     */
    void worker();

    /**
     * @brief Build SPI packet for current PWM state.
     *
     * @param buf Output buffer containing TLC59711 frame data.
     */
    void buildPacket(std::vector<uint8_t>& buf) const;

    /**
     * @brief Transmits a packet over SPI.
     *
     * @param buf Byte buffer to send.
     */
    void shiftOut(const std::vector<uint8_t>& buf) const;

    /**
     * @brief Convert normalized brightness to 16-bit PWM value.
     *
     * @param b Normalized brightness (0.0–1.0).
     * @return 16-bit PWM value.
     */
    static uint16_t toGS(Brightness b);

    /**
     * @brief Mapping from logical LED index to TLC59711 frame position.
     */
    static constexpr int FRAME_TO_GS[NUM_LEDS] = {
        0, 1, 2,
        3, 4, 5,
        6, 7
    };

    static constexpr int CHANNELS_PER_DRIVER = 12; 

    std::thread              _thread;       
    std::atomic<bool>        _running{false}; 

    
    std::mutex               _mutex;
    std::condition_variable  _cv;
    Channels                 _pending{};    
    bool                     _dirty{false}; 

    std::string              _spi_device;   
    uint32_t                 _spi_speed_hz{1'000'000};
    int                      _num_drivers{1}; 
    int                      _spi_fd{-1};   

    std::atomic<uint8_t>     _brightness{127}; 

    /**
     * @brief PWM buffer owned by worker thread.
     *
     * No locking required as it is not shared.
     */
    std::vector<uint16_t>    _pwm;
};
}

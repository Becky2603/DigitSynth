#include "tlc59711.hpp"

#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <iostream>

using namespace led_driver;
constexpr int TLC59711::FRAME_TO_GS[TLC59711::NUM_LEDS];

TLC59711::TLC59711(std::string spi_device, uint32_t spi_speed_hz, int num_drivers)
    : _spi_device(std::move(spi_device)),
      _spi_speed_hz(spi_speed_hz),
      _num_drivers(num_drivers),
      _pwm(num_drivers * CHANNELS_PER_DRIVER, 0)
{
    // The spidev device is opened in start() so unit tests that never call
    // start() can construct a TLC59711 without touching real hardware.
}

TLC59711::~TLC59711() {
    stop();
}

void TLC59711::start() {
    if (_running) return;

    _spi_fd = ::open(_spi_device.c_str(), O_WRONLY);
    if (_spi_fd < 0) {
        throw std::runtime_error("TLC59711: failed to open " + _spi_device +
                                 ": " + std::strerror(errno));
    }

    uint8_t  mode  = SPI_MODE_0;   // CPOL=0, CPHA=0 — TLC59711 samples on rising edge
    uint8_t  bits  = 8;
    uint32_t speed = _spi_speed_hz;

    if (::ioctl(_spi_fd, SPI_IOC_WR_MODE,          &mode)  < 0 ||
        ::ioctl(_spi_fd, SPI_IOC_WR_BITS_PER_WORD, &bits)  < 0 ||
        ::ioctl(_spi_fd, SPI_IOC_WR_MAX_SPEED_HZ,  &speed) < 0)
    {
        const int err = errno;
        ::close(_spi_fd);
        _spi_fd = -1;
        throw std::runtime_error(std::string("TLC59711: SPI ioctl config failed: ") +
                                 std::strerror(err));
    }

    _running = true;
    _thread  = std::thread(&TLC59711::worker, this);
}

void TLC59711::stop() {
    if (!_running) {
        if (_spi_fd >= 0) { ::close(_spi_fd); _spi_fd = -1; }
        return;
    }

    {
        std::lock_guard<std::mutex> lock(_mutex);
        _running = false;
    }
    _cv.notify_one();   // wake worker so it can see _running == false

    if (_thread.joinable())
        _thread.join();

    if (_spi_fd >= 0) {
        ::close(_spi_fd);
        _spi_fd = -1;
    }
}

void TLC59711::update(const Channels& channels) {
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if (_dirty) { return; }   // drop-if-busy, checked under the lock
        _pending = channels;
        _dirty   = true;
    }
    _cv.notify_one();
}

void TLC59711::setBrightness(uint8_t brightness) {
    _brightness = brightness;
}

void TLC59711::worker() {
    Channels channels{};

    while (true) {
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _cv.wait(lock, [this] { return _dirty || !_running; });

            if (!_running) break;

            channels = _pending;
            _dirty   = false;   // clear while lock is held — new updates can queue during SPI
        }
        // Lock is released here — calling thread is free immediately.

        std::fill(_pwm.begin(), _pwm.end(), 0);
        for (int i = 0; i < NUM_LEDS; ++i)
            _pwm[FRAME_TO_GS[i]] = toGS(channels[i]);

        std::vector<uint8_t> buf;
        buf.reserve(static_cast<size_t>(_num_drivers) * 28);
        buildPacket(buf);
        shiftOut(buf);
    }
    // No GPIO pins to clear — SPI hardware holds the lines idle on its own.
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

void TLC59711::shiftOut(const std::vector<uint8_t>& buf) const {
    if (_spi_fd < 0) return;   // safety — should not happen once start() succeeded

    struct spi_ioc_transfer xfer{};
    xfer.tx_buf        = reinterpret_cast<uintptr_t>(buf.data());
    xfer.rx_buf        = 0;                               // write-only
    xfer.len           = static_cast<uint32_t>(buf.size());
    xfer.speed_hz      = _spi_speed_hz;
    xfer.bits_per_word = 8;
    xfer.cs_change     = 0;

    if (::ioctl(_spi_fd, SPI_IOC_MESSAGE(1), &xfer) < 0) {
        std::cerr << "TLC59711: SPI transfer failed: "
                  << std::strerror(errno) << "\n";
    }
    // The TLC59711 latches GS data after the final bit is clocked in because
    // the command word sets TMGRST=1 and DSPRPT=1. The natural gap between
    // successive update() calls provides the required clock-idle interval;
    // no manual hold delay is needed.
}

uint16_t TLC59711::toGS(Brightness b) {
    if (b <= 0.0f) return 0;
    if (b >= 1.0f) return 0xFFFF;
    return static_cast<uint16_t>(b * 65535.0f + 0.5f);
}
#include "spi.h"
#include "types.h"

#include <cerrno>
#include <cstdint>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <vector>

#include <sys/types.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

void checkError(int result) {
    if (result < 0) {
        std::cerr << strerror(errno) << std::endl;
        exit(result);
    }
}

Spi::Spi(std::string path, SpiSettings settings) {
    this->fd = open(path.c_str(), O_RDWR);
    checkError(this->fd);
    
    this->worker = std::thread([this] () {
        
        while (true) {
            std::unique_lock<std::mutex> lock(this->mut);
            // Wake if the queues have data 
            std::cout << "hellow3\n";
            this->cond.wait(lock, [this] { return !this->readQueue.empty() || !this->writeQueue.empty(); });
            
            while (!this->readQueue.empty()) {
                auto tuple = &this->readQueue.front();
                
                std::vector<uint8_t> *vec = std::get<0>(*tuple);
                auto buf = vec->data();
                auto len  = vec->size(); 
                // todo: set cs pin
                ssize_t bytesRead = ::read(this->fd, buf, len);
                std::cout << bytesRead << std::endl;
                auto callback = std::get<2>(*tuple);
                callback(bytesRead);
                
                this->readQueue.pop_front();
            }
            
            while (!this->writeQueue.empty()) {
                auto tuple = &this->writeQueue.front();
                
                auto vec = std::get<0>(*tuple);
                auto buf = vec.data();
                auto len  = vec.size(); 
                // todo: set cs pin
                while ((size_t) ::write(this->fd, buf, len) != len) {}
                
                this->writeQueue.pop_front();
            }
        }
    });

    updateSettings(settings);
}

Spi::~Spi() {
    checkError(close(this->fd));
    this->worker.join();
}

void Spi::updateSettings(SpiSettings settings) {
    uint8_t mode = (settings.clockPolarity << 1) & (settings.clockPhase);
    uint8_t bitsPerWord = settings.bitsPerWord;
    uint8_t bitOrder = settings.bitOrder;

    checkError(ioctl(fd, SPI_IOC_WR_MODE,          &mode));
    checkError(ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bitsPerWord));
    checkError(ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ,  &settings.clockFrequency));
    checkError(ioctl(fd, SPI_IOC_WR_LSB_FIRST,     &bitOrder));
}

void Spi::read(std::vector<uint8_t> *dest, SpiDevice device, SpiCallback callback) {
    this->mut.lock();
    this->readQueue.push_back(std::make_tuple(dest, device, callback));
    this->cond.notify_one();
    this->mut.unlock();
}

void Spi::write(std::vector<uint8_t> src, SpiDevice device) {
    this->mut.lock();
    this->writeQueue.push_back(std::make_tuple(src, device));
    this->cond.notify_one();
    this->mut.unlock();
}

std::optional<SpiDevice> Spi::addDevice() {
    if (this->nDevices >= SPI_MAX_DEVICES) { return {}; }

    auto spiDevice = (SpiDevice) this->nDevices;
    this->nDevices++;

    return spiDevice;
}

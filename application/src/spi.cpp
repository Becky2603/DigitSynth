#include "spi.h"
#include "types.h"

#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <linux/spi/spi.h>
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
        fflush(stdout);
        fflush(stderr);
        exit(result);
    }
}

Spi::Spi(std::string path, SpiSettings settings) {
    this->fd = open(path.c_str(), O_RDWR);
    std::cout << "opened file\n";
    checkError(this->fd);
    
    this->worker = std::thread([this] () {
        
        while (true) {
            std::unique_lock<std::mutex> lock(this->mut);
            // Wake if the queues have data 
            this->cond.wait(lock, [this] { return !this->readQueue.empty() || !this->writeQueue.empty(); });
            
            while (!this->readQueue.empty()) {
                auto tuple = &this->readQueue.front();
                
                std::vector<uint8_t> *vec = std::get<0>(*tuple);
                auto buf = vec->data();
                auto len = vec->size(); 
                std::cout << "spi vec len: " << len << std::endl;
                std::cout << "spi device: " << std::get<1>(*tuple) << std::endl;
                // todo: set cs pin
                printf("%lx\n", (unsigned long) buf);
                ssize_t bytesRead = ::read(this->fd, buf, len);
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
    // uint8_t mode = (settings.clockPolarity << 1) & (settings.clockPhase);
    uint8_t mode = SPI_MODE_1;
    uint8_t bitsPerWord = settings.bitsPerWord;
    // uint8_t bitOrder = settings.bitOrder;

    std::cout << "setting mode\n";
    checkError(ioctl(fd, SPI_IOC_WR_MODE,          &mode));
    std::cout << "set mode\n";
    checkError(ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bitsPerWord));
    std::cout << "set bits per word\n";
    checkError(ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ,  &settings.clockFrequency));
    std::cout << "set clock\n";
    // checkError(ioctl(fd, SPI_IOC_WR_LSB_FIRST,     &bitOrder));
    // std::cout << "set bit order\n";
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

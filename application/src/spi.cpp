#include "spi.h"
#include "types.h"

#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <linux/spi/spi.h>
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
    updateSettings(settings);
}

Spi::~Spi() {
    checkError(close(this->fd));
}

void Spi::updateSettings(SpiSettings settings) {
    uint8_t mode = SPI_MODE_1;
    uint8_t bitsPerWord = settings.bitsPerWord;

    checkError(ioctl(fd, SPI_IOC_WR_MODE,          &mode));
    checkError(ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bitsPerWord));
    checkError(ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ,  &settings.clockFrequency));
}

void Spi::read(std::vector<uint8_t> dest) {
    ::read(this->fd, dest.data(), dest.size());
}

void Spi::write(std::vector<uint8_t> src) {
    ::write(this->fd, src.data(), src.size());
}

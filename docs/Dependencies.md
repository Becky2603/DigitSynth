# Dependencies

This document describes all dependencies required to build and run DigitSynth on Raspberry Pi.

A full installation script is available at `application/install-deps.sh`.

---

## System Packages

Installed via `apt`:

| Package | Purpose |
|---|---|
| `autotools-dev` | Build toolchain support for autoconf-based projects |
| `autoconf-archive` | Extended macro library for autoconf |
| `libtool` | Generic shared library support |
| `pkg-config` | Compiler flag helper for installed libraries |
| `alsa` | ALSA sound system |
| `libasound2-dev` | ALSA development headers (required by RtMidi) |

---

## libgpiod

**Version:** 2.2
**Source:** https://mirrors.edge.kernel.org/pub/software/libs/libgpiod/
**Purpose:** Linux GPIO access from userspace — used for hardware pin control on the Raspberry Pi.

### Installation

```bash
curl -S https://mirrors.edge.kernel.org/pub/software/libs/libgpiod/libgpiod-2.2.tar.gz -o libgpiod.tar.gz
tar -xf libgpiod.tar.gz
cd libgpiod-2.2/
./configure --enable-bindings-cxx
make
sudo make install
```

> The `--enable-bindings-cxx` flag is required to build the C++ bindings used by this project.

---

## ADS1115 Raspberry Pi Driver

**Source:** https://github.com/berndporr/rpi_ads1115
**Purpose:** Userspace driver for the ADS1115 16-bit ADC over I2C — used to read analogue input from the flex sensors and potentiometers via SPI.

### Installation

```bash
git clone https://github.com/berndporr/rpi_ads1115.git
cd rpi_ads1115/
cmake .
make
sudo make install
```

> Requires I2C to be enabled on the Raspberry Pi. Enable via `sudo raspi-config` → Interface Options → I2C.

---

## iir1 — IIR Filter Library

**Source:** https://github.com/berndporr/iir1
**Purpose:** Digital IIR filter design and processing — used in `FlexDSP` to apply a 4th order Butterworth low-pass filter to flex sensor data.

### Installation

```bash
git clone https://github.com/berndporr/iir1.git
cd iir1
cmake .
make
sudo make install
```

### Usage in this project

Include the filter header with:

```cpp
#include <iir/Butterworth.h>
```

---

## RtMidi

**Source:** https://github.com/thestk/rtmidi
**Purpose:** Real-time MIDI input/output — used to send MIDI CC messages from the glove controller to a connected synthesiser.

### Installation

```bash
git clone https://github.com/thestk/rtmidi.git
cd rtmidi
cmake . -DRTMIDI_API_ALSA=ON -DRTMIDI_API_JACK=OFF
make
sudo make install
```

> Built with ALSA backend enabled and JACK disabled. ALSA is the default audio/MIDI layer on Raspberry Pi OS.

---

## Notes

- All libraries are installed system-wide via `sudo make install` and are picked up automatically by CMake.
- If CMake cannot find a library after installation, run `sudo ldconfig` to refresh the dynamic linker cache.
- Tested on Raspberry Pi OS 64-bit (Bookworm).

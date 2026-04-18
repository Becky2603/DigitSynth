#!/usr/bin/env bash

sudo apt install -y autotools-dev autoconf-archive libtool pkg-config alsa libasound2-dev

# libgpiod
curl -S https://mirrors.edge.kernel.org/pub/software/libs/libgpiod/libgpiod-2.2.tar.gz -o libgpiod.tar.gz
tar -xf libgpiod.tar.gz
cd libgpiod-2.2/
./configure --enable-bindings-cxx
make
sudo make install 
cd -

# ADS1115 driver
git clone https://github.com/berndporr/rpi_ads1115.git
cd rpi_ads1115/
cmake .
make
sudo make install
cd -

# IIR filter
git clone https://github.com/berndporr/iir1.git
cd iir1
cmake .
make
sudo make install
cd - 

# RtMidi
git clone https://github.com/thestk/rtmidi.git
cd rtmidi
cmake . -DRTMIDI_API_ALSA=ON -DRTMIDI_API_JACK=OFF
make
sudo make install
cd -

# DigitSynth

> A real-time wearable audio effects processor built on Raspberry Pi — apply filters and effects to live audio using MIDI control messages with responsive LEDs.

---
##  Features

-  Real-time EQ and filtering control for a MIDI synthesiser
-  Low-latency digital signal processing on Raspberry Pi
-  Hardware controls for intuitive parameter adjustment
-  MIDI CC output with adjustable filter parameters
-  Responsive LEDs that react in real-time to parameter changes
-  Open source and fully reproducible
---
## Hardware Requirements
 
### Bill of Materials

The table below outlines the hardware components used in this project. Exact component matches are not required, and suitable alternatives will yield comparable results. However, as the software has been developed specifically for the Raspberry Pi, it is strongly recommended to retain this as the target platform.
 
| Component | Part Number | Quantity | Approx. Cost (unit)|
|---|---|---|---|
| Raspberry Pi 5 | Pi 5 8GB | 1 | £161.53 |
| Adafruit ADS1115 | 1085 | 1 | £11.18 |
| Adafruit TLC59711 | 1455 | 1 | £6.73 |
| Spectra Symbol Flex Sensor FLX| FLX-L-055-123-MP | 4 | £8.51 |
| Tactile Switch, 4.3mm, 160g | MCDTS2-1N| 4 | £0.17 |
| Kingbright L-7113IT 5mm 2V Red LED 80mcd | 56-0250 | 8 | £0.08 |
| Pisugar S Plus 5000 mAh Raspberry Pi UPS | Pisugar S Plus | 1 | $29.99 |

A MIDI-receiving device is required to complete the setup, either a hardware synthesiser or a software synthesiser running on a connected laptop. In this project, a Roland JD-Xi synthesiser was used, receiving MIDI directly from the Raspberry Pi over USB-B.

### Wiring Diagram:

![Wiring diagram](docs/wiring-diagram.jpg)

### Resistor Selection & Voltage Divider Justification

While the flex sensors used in this project have a flat resistance of approximately 12 kΩ (±20%), this value is not universal across all flex sensors. Be sure to check your datasheet before substituting.

**Sensor operating range:** The flat resistance sits in the range 9.6 kΩ – 14.4 kΩ. At full deflection (180°, 8 mm bend radius), the resistance increases by a factor of >4×, giving an estimated fully-bent resistance of ~48 kΩ. This yields a total operating range of roughly 10 kΩ – 50 kΩ.

**Voltage divider:** A fixed resistor of R₁ = 10 kΩ was chosen, with the flex sensor as R₂ and a 5 V supply:

$$V_{out} = \frac{R_2}{R_1 + R_2} \times V_{in}$$

| State | R₂ | V_out |
|---|---|---|
| Flat | 12 kΩ | 2.73 V |
| Fully bent | 48 kΩ | 4.14 V |

This gives a swing of 1.41 V — a usable and easily resolvable range for the ADC. The maximum output (4.14 V) also remains safely below the ADS1115 maximum analog input voltage of V_DD + 0.3 V = 5.3 V.

**ADC source impedance:** The worst-case source impedance seen by the ADC occurs when the flex sensor is at maximum resistance:

$$R_{source} = R_1 \| R_2 = \left(\frac{1}{10k} + \frac{1}{50k}\right)^{-1} = 8.33\ k\Omega$$

The ADS1115 is a delta-sigma ADC with an input impedance in the range of 10–22 MΩ (depending on PGA gain setting). At 8.33 kΩ, the source impedance is more than three orders of magnitude below this, making its effect on measurement accuracy negligible.

The ADC automatically determines the minimum and maximum voltages, allowing the voltage range to be suitably scaled to the flex sensor.
---
## Software Requirements

### Toolchain
Our build system is CMake, but we provide a simple top-level Makefile to make things easy. Feel free to run the `cmake` commands manually if you are so inclined. 

### 1. Clone the Repository
 
```bash
git clone https://github.com/RTES-Group/DigitSynth.git
cd DigitSynth
```

### 2. Install Dependencies

#### Dependencies

| Library | Purpose | Source |
|---|---|---|
| IIR Filter Library | Digital IIR filter design and processing | [berndporr/iir1](https://github.com/berndporr/iir1) |
| RtMidi | Real-time MIDI input/output | [thestk/rtmidi](https://github.com/thestk/rtmidi) |
| ADS1115 API | Flex sensor measurement | [berndporr/rpi_ads1115](github.com/berndporr/rpi_ads1115) |
| libgpiod | GPIO management | [ligpiod](https://libgpiod.readthedocs.io/en/latest/building.html) | 
| libasound2-dev | ALSA development headers | [libasound2-dev](https://alsa-project.org/) |

Simply run the `install-deps.sh` script:
```bash 
./install-deps.sh
```

### 3. Enable SPI on the Raspberry Pi:
SPI must be enabled before the project will run:

```bash
sudo raspi-config
gpio i2cd
# Navigate to: Interface Options → SPI → Enable
```

### 4. Build the project:

```bash
make
```

### 5. Run:
```bash
build/digitsynth
```

---

## Unit tests

To run unit tests: 
```bash 
make test
```

---

## Contributing

See the issues tab for a list of potential contributions. In particular, we would welcome expanding support for other synths. 

---

## Social Media

Follow our build journey on Instagram: [![Instagram](https://img.shields.io/badge/Instagram-%23E1306C.svg?style=flat&logo=instagram&logoColor=white)](https://instagram.com/digitsynth_)

---
## License

This project is licensed under the **MIT License** — see the [`LICENSE`](LICENSE) file for details.
 
You are free to use, modify, and distribute this project. We just ask that you credit the original authors.

---

## Authors & Acknowledgements

Developed by:

- **Logan Brown** — [GitHub](https://github.com/LoganBrownGU)
- **Becky Clarke** — [GitHub](https://github.com/Becky2603)
- **Dougal Harris** — [GitHub](https://github.com/DougalH1)
- **Finn McConville** — [GitHub](https://github.com/finnmcco)
- **Jamie Wedlinscky** — [GitHub](https://github.com/jwedlinscky)
  
As part of the **Real-Time Embedded Systems** module at **University of Glasgow**, *2026*.








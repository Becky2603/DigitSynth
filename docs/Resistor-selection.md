
### Resistor Selection & Voltage Divider Justification

While the flex sensors used in this project have a flat resistance of approximately 12 kΩ (±20%), this value is not universal across all flex sensors. Be sure to check your datasheet before substituting.

**Sensor operating range:** The flat resistance sits in the range 9.6 kΩ – 14.4 kΩ. At full deflection (180°, 8 mm bend radius), the resistance increases by a factor of >4×. We measured the fully-bent resistance to be ~50 kΩ. This yields a total operating range of 9.6 kΩ – 50 kΩ.

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

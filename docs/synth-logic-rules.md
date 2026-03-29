# Synth Logic Rules (Initial Iteration)

These rules are defined around a synthesiser consisting of four composite waveforms (produces a rich sound) with a chord playing continuously with infinite sustain (drone sound).

There are four control modes:

## Mode 1: EQ Mode
Selected by pressing Button 1  
Each flex sensor controls the attenuation of one EQ frequency band (4 bands total - one for each sensor)

## Mode 2: Source EQ Mode
Selected by pressing Button 2  
Each flex sensor controls the lowpass filter cutoff of one of the four source waveforms

## Mode 3: Detune Mode
Selected by pressing Button 3  
Each flex sensor controls the pitch detune of one source waveform  
Detune is mapped over the range 0 to -100 cents

## Mode 4: Chord Mode
Selected by pressing Button 4  
The four flex sensors continue controlling the same parameters as in the previously active control mode  
Pressing buttons no longer switches control mode  
Instead, the buttons each select one of four predefined chord presets to be played by the synthesiser  
To exit chord mode, all four buttons must be pressed simultaneously. The system resets to its previously active control mode

## Button Handling Rules
A single button press triggers its associated action immediately, not after release  
Multi-button presses are ignored, except for the four-button reset used to exit chord mode

A few of the LEDs could be used to indicate the currently active control mode!

## Flex Sensor Scaling
Flex sensor values are mapped linearly into the MIDI range of 0-127.

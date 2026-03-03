# MIDI Encoding Thread

## Responsibility

Responsibility: turn information on flex sensor extension and button presses into relevant MIDI data.

## Inputs

Button states (pressed / not pressed x4)  
Some kind of measurement of flex sensor extension (e.g. voltage or cm)

## Processing

Process button data  
This determines what parameters on the synth are controlled by the flex sensors  
E.g. when button 1 is pressed, the flex sensors behave as EQs for different frequency bands; when button 2 is pressed, the function of the flex sensors changes to detuning frequency components of the synth.  

Process flex sensor data  
Convert flex sensor readings to MIDI range (7-bit integer)  
Format MIDI messages if necessary

## Output

The thread outputs MIDI “Control Change” messages with controller numbers dependent on what function the flex sensors are performing (as configured by button presses).  

Controller numbers are arbitrary; their functions can be determined when they are parsed by the synthesiser.

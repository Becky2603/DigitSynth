# Design documents

## Software architecture
1. Overview

   4 analogue sensors (FSRs) and 4 digital inputs (buttons) connect to a Raspberry Pi. The digital sensors control which MIDI control parameters are active. The analogue sensors will incrementally adjust the control parameters for different levels of the effects. 

2. Setup

 ```
 ON STARTUP: 

  FOR each analogue sensor (0 to 3): 

    set up ADC channel 

    assign value-change callback


  FOR each digital button (0 to 3): 

  set up GPIO pin 

  attach interrupt on PRESS and RELEASE


  set active_parameter = PARAMETER_0  (default) 

  open MIDI output port 

  start analog sample timer (every 10ms) 

  start event worker thread 

  wait for signals (SIGINT / SIGTERM)

```

  3. Analogue Sensors

     Each analogue sensor reads a raw value from 0 to 1023. That value is scaled to a MIDI CC value (0-127) and sent to whichever control parameter is currently selected. The digital buttons determine which parameter that is.
     
```
EVERY 10ms (timer interrupt): 

  FOR each analogue sensor: 

    read raw value from ADC (0 to 1023) 

    smooth the value (rolling average) 

    scale to MIDI range (0 to 127) 

    push ANALOGUE_UPDATE event to queue 

      -- includes: sensor_id, scaled value
```
  4. Digital Buttons

     Each button fires a hardware interrupt on press and release. The interrupt only pushes an event to the queue. Each button press selects a control parameter. The analogue sensors then update that parameter until a different button is pressed. Button release has no effect on parameter selection.

```
ON BUTTON PRESS (hardware interrupt): 

  look up which control parameter this button maps to 

  push PARAMETER_SELECT event to queue 

    -- includes: button_id, parameter_id 

  

ON BUTTON RELEASE (hardware interrupt): 

  push BUTTON_RELEASE event to queue 

    -- (behaviour TBD e.g. latch or hold mode)
```

5. Event Worker Thread

   A background thread processes all events from the queue and sends MIDI. This keeps interrupt handlers fast. 

  
```
LOOP (worker thread): 

  pop next event from queue 

  IF no event: wait 1ms, try again 

  

  IF event is PARAMETER_SELECT: 

    set active_parameter = event.parameter_id 

  

  IF event is ANALOGUE_UPDATE: 

    send MIDI CC for active_parameter 

      using CC_MAP[active_parameter] and event.value 

  

  IF event is BUTTON_RELEASE: 

    (handle latch/hold logic if needed) 
```
6. Shutdown

```
   ON SIGINT or SIGTERM: 

  cancel timer 

  stop worker thread 

  close MIDI port 

  exit cleanly
```
7. Midi Mapping TBD

   Exact CC numbers and parameter names to be confirmed once the target synth is decided.

```
   BUTTON_MAP  (button -> control parameter): 

  Button 0 -> Parameter A  (e.g. Filter Cutoff) 

  Button 1 -> Parameter B  (e.g. Resonance) 

  Button 2 -> Parameter C  (e.g. Reverb Send) 

  Button 3 -> Parameter D  (e.g. LFO Rate) 

  

CC_MAP  (parameter -> MIDI CC number): 

  Parameter A -> CC TBD 

  Parameter B -> CC TBD 

  Parameter C -> CC TBD 

  Parameter D -> CC TBD
```
8. Classes

   - AnalogSensor - ADC read, smoothing, value scaling 

   - DigitalButton - GPIO setup, ISR, debounce 

   - EventQueue - thread-safe queue with mutex 

   - MidiController - sends CC values, manages parameter mapping 
 
## Hardware architecture
## Circuit diagrams 

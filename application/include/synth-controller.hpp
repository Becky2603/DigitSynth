#ifndef SynthController_hpp
#define SynthController_hpp

#include "mode-manager.hpp"
#include "button-driver.hpp"
#include "message-builder.hpp"
#include "midi-scaler.hpp"
#include "chord-manager.hpp"
#include "led-controller.hpp"
#include "flex-sensor.hpp"
#include "midi-driver.hpp"
#include "i-led-driver.hpp"
#include "patterns.hpp"
#include "lfo-manager.hpp"
#include "flex-dsp.hpp"
#include <memory>

/**
 * Top level coordinator for DigitSynth.
 *
 * Owns and wires together all synth logic components (ModeManager, ChordManager,
 * LfoManager, MessageBuilder, LedController) and hardware driver abstractions
 * (IButtonDriver, IFlexSensor, IMidiDriver, ILedDriver, IPattern).
 *
 * On construction, SynthController:
 *  - Opens the MIDI output port
 *  - Plays the default boot chord (Am11)
 *  - Registers button and flex sensor callbacks
 *
 * Button behaviour (NORMAL mode):
 *  - Button 0: enter chord mode
 *  - Button 1: toggle LFO enabled/disabled
 *  - Button 2: cycle LFO shape, sends CC 3 to JD-Xi
 *  - Button 3: toggle LED pattern (STATUS / RIPPLE)
 *
 * Button behaviour (CHORD mode):
 *  - Buttons 0–3: select chord; pressing the active chord exits chord mode
 *
 * Flex sensor behaviour:
 *  - Sensor 0: filter cutoff (CC 74)
 *  - Sensor 1: LFO rate (NRPN)
 *  - Sensor 2: LFO filter depth (NRPN), fixed at 64 when LFO is disabled
 *  - Sensor 3: pitch bend
 *
 * All hardware dependencies are injected via the constructor, following the
 * Dependency Inversion Principle and enabling integration testing with mocks.
 */
class SynthController {
public:
    /**
     * Constructs and initialises the SynthController.
     *
     * Takes ownership of buttonDriver, flexSensor and midiDriver via unique_ptr.
     * tlc and pattern are held by reference
     *
     * @param tlc          Reference to the LED driver (real or mock)
     * @param pattern      Reference to the LED ripple pattern (real or mock)
     * @param buttonDriver Owning pointer to the button driver (real or mock)
     * @param flexSensor   Owning pointer to the flex sensor / DSP pipeline (real or mock)
     * @param midiDriver   Owning pointer to the MIDI output driver (real or mock)
     */
    explicit SynthController(
        led_driver::ILedDriver&                       tlc,
        led_pattern::IPattern&                        pattern,
        std::unique_ptr<button_driver::IButtonDriver> buttonDriver,
        std::unique_ptr<flex_sensor::IFlexSensor>     flexSensor,
        std::unique_ptr<midi_driver::IMidiDriver>     midiDriver
    );

    /**
     * Destructor — sends note-off messages for all notes in the current chord
     * before shutdown, preventing stuck notes on the JD-Xi.
     */
    ~SynthController();

private:
    ModeManager    modeManager;
    ChordManager   chordManager;
    MessageBuilder messageBuilder;
    MidiScaler     midiScaler;
    LfoManager     lfoManager;

    std::unique_ptr<midi_driver::IMidiDriver>     midiDriver;
    LedController                                 ledController;
    std::unique_ptr<button_driver::IButtonDriver> buttonDriver;
    std::unique_ptr<led_driver::ILedDriver>       tlc;
    FlexDSP                                       flexDSP;
};

#endif /* SynthController_hpp */

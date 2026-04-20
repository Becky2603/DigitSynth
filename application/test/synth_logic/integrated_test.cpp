#include <cassert>
#include <iostream>
#include <memory>
#include <vector>
#include <array>

#include "MockPattern.hpp"
#include "SynthController.hpp"
#include "MockTLC59711.hpp"
#include "MidiTypes.hpp"
#include "button-driver.h"
#include "flex-sensor.h"
#include "midi-driver.hpp"

namespace button_driver {
class MockButtonDriver : public IButtonDriver {
public:
    void registerSingleButtonCallback(SingleButtonCallback cb) override { singleCallback = cb; }
    void deregisterSingleButtonCallback() override {}
    void registerAllButtonsCallback(AllButtonsCallback cb) override {(void)cb; }
    void deregisterAllButtonsCallback() override {}
    void simulatePress(size_t index) { if (singleCallback) singleCallback(index); }
    SingleButtonCallback singleCallback;
};
}

namespace flex_sensor {
class MockFlexSensor : public IFlexSensor {
public:
    void registerCallback(ExtensionCallback cb) override { this->callback = cb; }
    void begin() override {}
    void simulateReading(std::array<float, 4> values) { if (this->callback.has_value()) { callback.value()(values); } }
    std::optional<ExtensionCallback> callback;
};
}

namespace midi_driver {
class MockMidiDriver : public IMidiDriver {
public:
    std::vector<std::string> listOutputPorts() override { return {"MockPort"}; }
    void openPort(unsigned int) override {}
    void sendMessage(const midi_message& msg) override { 
        sentMessages.push_back(msg); 
    }
    std::vector<midi_message> sentMessages;
};
}

int main() {
    auto mockMidi    = new midi_driver::MockMidiDriver();
    auto mockTlc     = led_driver::MockTLC59711();
    auto mockPattern = MockPattern(mockTlc);
    auto mockButtons = new button_driver::MockButtonDriver();
    auto mockFlex    = new flex_sensor::MockFlexSensor();

    SynthController synth(
        mockTlc,
        mockPattern,
        std::unique_ptr<button_driver::IButtonDriver>(mockButtons), 
        std::unique_ptr<flex_sensor::IFlexSensor>(mockFlex),
        std::unique_ptr<midi_driver::IMidiDriver>(mockMidi)
    );

    //Am11 should have been sent on construction
    // 6 note-on messages expected
    std::cout << mockMidi->sentMessages.size() << std::endl;
    assert(mockMidi->sentMessages.size() == 6);
    for (int i = 0; i < 6; i++) {
        assert(mockMidi->sentMessages[i].status == 0x90);
    }
    mockMidi->sentMessages.clear();

    //Test 1: Button 0 in NORMAL mode enters CHORD mode
    mockButtons->simulatePress(0);
    //No midi messages expected from mode switch alone
    //Verify by pressing a chord button and checking note-ons are sent
    mockButtons->simulatePress(3); // select chord 3
    // should send 6 note ons
    int noteOns = 0;
    for (auto& msg : mockMidi->sentMessages) {
        if (msg.status == 0x90) noteOns++;
    }
    assert(noteOns == 6);
    mockMidi->sentMessages.clear();

    //Test 2: Switching chord sends note offs then note ons
    mockButtons->simulatePress(2); // switch to chord 2
    int noteOffs = 0;
    noteOns = 0;
    for (auto& msg : mockMidi->sentMessages) {
        if (msg.status == 0x80) noteOffs++;
        if (msg.status == 0x90) noteOns++;
    }
    assert(noteOffs == 6);
    assert(noteOns == 6);
    mockMidi->sentMessages.clear();

    //Test 3: Pressing same chord button exits chord mode
    mockButtons->simulatePress(2);
    // Verify we're back in normal mode by pressing button 1 (LFO toggle)
    mockMidi->sentMessages.clear();
    mockButtons->simulatePress(1); //LFO toggle — no midi in normal mode
    // so sentMessages should still be empty
    assert(mockMidi->sentMessages.size() == 0);

    //Test 4: Button 2 in NORMAL mode sends LFO shape CC
    mockButtons->simulatePress(2); //cycle LFO shape
    assert(mockMidi->sentMessages.size() == 1);
    assert(mockMidi->sentMessages[0].status == 0xB0);
    assert(mockMidi->sentMessages[0].data_1 == 3); //CC 3 = LFO shape
    mockMidi->sentMessages.clear();

    //Test 5: Flex sensor in NORMAL mode sends CC messages
    mockFlex->simulateReading({0.5f, 0.5f, 0.5f, 0.5f});
    // index 0 => CC (cutoff), index 1 => NRPN (LFO rate, 4 msgs),
    // index 2 => NRPN (LFO depth, 4 msgs), index 3 => pitch bend
    // total = 10 messages
    assert(mockMidi->sentMessages.size() == 10);
    assert(mockMidi->sentMessages[0].status == 0xB0); // cutoff CC
    assert(mockMidi->sentMessages[0].data_1 == 74);   // CC 74
    assert(mockMidi->sentMessages[9].status == 0xE0); // pitch bend
    mockMidi->sentMessages.clear();

    return 0;
}

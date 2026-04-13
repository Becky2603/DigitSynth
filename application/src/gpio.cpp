#include "gpio.h"
#include "types.h"
#include <chrono>
#include <cstring>
#include <gpiod.hpp>
#include <iostream>
#include <optional>

static gpiod::chip *chip;

static std::optional<GpioCallback> callback = {};

void gpio::setupGpio() {
    chip = new gpiod::chip("/dev/gpiochip0");
    if (chip == nullptr) {
        std::cerr << "Could not open GPIO device\n";
        exit(-1);
    }
}

void gpio::teardownGpio() {
    chip->close();
}

void gpio::setPin(int pin, bool value) {
    if (chip == nullptr) {
        std::cerr << "GPIO chip not set up\n";
        exit(-1);
    }
    
    gpiod::line_config line_config; 
    line_config.add_line_settings(pin, gpiod::line_settings().set_direction(gpiod::line::direction::OUTPUT));
    auto builder = chip->prepare_request();
    builder
        .set_consumer("digitsynth output")
        .set_line_config(line_config);
    
    auto request = gpiod::line_request(builder.do_request());
    request.set_value(pin, gpiod::line::value(value));
}

bool gpio::getPin(int pin) {
    if (chip == nullptr) {
        std::cerr << "GPIO chip not set up\n";
        exit(-1);
    }   
    
    gpiod::line_config line_config; 
    line_config.add_line_settings(pin, gpiod::line_settings().set_direction(gpiod::line::direction::INPUT));
    auto builder = chip->prepare_request();
    builder
        .set_consumer("digitsynth input")
        .set_line_config(line_config);
    
    auto request = gpiod::line_request(builder.do_request());
    bool value = (bool) request.get_value(pin);
    
    return value;
}

gpiod::edge_event::event_type gpio::blockUntilEdge(int pin, gpiod::line::edge edge) {
    if (chip == nullptr) {
        std::cerr << "GPIO chip not set up\n";
        exit(-1);
    }   
    
    gpiod::line_config line_config; 
    line_config.add_line_settings(pin, gpiod::line_settings().set_direction(gpiod::line::direction::INPUT).set_edge_detection(edge).set_bias(gpiod::line::bias::PULL_DOWN));
    auto builder = chip->prepare_request();
    builder
        .set_consumer("digitsynth callback")
        .set_line_config(line_config);
    
    auto request = gpiod::line_request(builder.do_request());
    
    bool r = false;
    
    while (!r) {
        r = request.wait_edge_events(std::chrono::milliseconds(5000));
    }
    gpiod::edge_event_buffer buf; 
    request.read_edge_events(buf, 1);
    auto e = buf.get_event(0);
    return e.type();
}

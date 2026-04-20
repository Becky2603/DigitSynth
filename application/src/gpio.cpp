#include "gpio.hpp"
#include <chrono>
#include <cstring>
#include <gpiod.hpp>
#include <iostream>
#include <optional>

#define BLOCK_TIMEOUT_MS    (500)

static gpiod::chip *chip;

static bool lineRequestsRunning = false; 

void gpio::setupGpio() {
    chip = new gpiod::chip("/dev/gpiochip0");
    if (chip == nullptr) {
        std::cerr << "Could not open GPIO device\n";
        exit(-1);
    }
    
    lineRequestsRunning = true;
}

void gpio::teardownGpio() {
    gpio::cancelLineRequests();
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

std::optional<gpiod::edge_event::event_type> gpio::blockUntilEdge(int pin, gpiod::line::edge edge) {
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
    
    auto rq = gpiod::line_request(builder.do_request());
    bool r = false;
    
    while (!r && lineRequestsRunning) {
        r = rq.wait_edge_events(std::chrono::milliseconds(BLOCK_TIMEOUT_MS));
    }
    
    if (!lineRequestsRunning) { return {}; }
    
    gpiod::edge_event_buffer buf; 
    rq.read_edge_events(buf, 1);
    auto e = buf.get_event(0);
    
    return e.type();
}

void gpio::cancelLineRequests() {
    lineRequestsRunning = false;
}

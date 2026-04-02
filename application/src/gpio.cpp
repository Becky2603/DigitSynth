#include "gpio.h"
#include "types.h"
#include <chrono>
#include <cstring>
#include <gpiod.hpp>
#include <iostream>
#include <optional>
#include <thread>

static gpiod::chip *chip;

static std::thread t; 
static std::optional<GpioCallback> callback = {};
static std::optional<gpiod::line_request *> callbackRequest = {};

void gpio::setupGpio() {
    chip = new gpiod::chip("/dev/gpiochip0");
    if (chip == nullptr) {
        std::cerr << "Could not open GPIO device\n";
        exit(-1);
    }
    
    t = std::thread([] () {
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            
            while (::callbackRequest.has_value()) {
                bool r = ::callbackRequest.value()->wait_edge_events(std::chrono::milliseconds(200));
                if (r && ::callback.has_value()) {
                    gpiod::edge_event_buffer buf;
                    ::callbackRequest.value()->read_edge_events(buf, 1);
                    ::callback.value()();
                    
                    ::callback = {};
                }
            }
        }
    });
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

void gpio::blockUntilEdge(int pin, gpiod::line::edge edge) {
    if (chip == nullptr) {
        std::cerr << "GPIO chip not set up\n";
        exit(-1);
    }   
    
    gpiod::line_config line_config; 
    line_config.add_line_settings(pin, gpiod::line_settings().set_direction(gpiod::line::direction::INPUT).set_edge_detection(edge));
    auto builder = chip->prepare_request();
    builder
        .set_consumer("digitsynth callback")
        .set_line_config(line_config);
    
    auto request = new gpiod::line_request(builder.do_request());
    
    bool r = false;
    
    while (!r) {
        r = request->wait_edge_events(std::chrono::milliseconds(500));
    }
    gpiod::edge_event_buffer buf; 
    request->read_edge_events(buf, 1);
}

void gpio::registerCallback(int pin, gpiod::line::edge edge, GpioCallback callback) {
    if (chip == nullptr) {
        std::cerr << "GPIO chip not set up\n";
        exit(-1);
    }   
    
    gpiod::line_config line_config; 
    line_config.add_line_settings(pin, gpiod::line_settings().set_direction(gpiod::line::direction::INPUT).set_edge_detection(edge));
    auto builder = chip->prepare_request();
    builder
        .set_consumer("digitsynth callback")
        .set_line_config(line_config);
    
    auto request = new gpiod::line_request(builder.do_request());
    ::callback = callback;
    ::callbackRequest = request;
}

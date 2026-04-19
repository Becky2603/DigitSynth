cmake_minimum_required(VERSION 3.23)

enable_testing()

function(create_test EXE NAME FILE)
    add_executable(${EXE} ${FILE})
    target_include_directories(${EXE} PRIVATE include)
    target_include_directories(${EXE} PRIVATE {CMAKE_SOURCE_DIR}/test/mocks)
    link_directories("/usr/local/lib/")
    target_link_libraries(${EXE} digitsynth_static gpiod gpiodcxx ads1115rpi rtmidi)
    add_test(NAME ${NAME} COMMAND ${EXE})
endfunction()

create_test(test-foo TestFoo test_foo.cpp)
create_test(test-flex-sensor TestFlexSensor test_flex-sensor.cpp)
create_test(test-voltage-scaler TestVoltageScaler test_voltage-scaler.cpp)
create_test(test-gpio TestGpio test_gpio.cpp)
# create_test(test_led_controller TestLedController synth_logic/led_controller_test.cpp)

cmake_minimum_required(VERSION 3.23)

enable_testing()

<<<<<<< HEAD
# test_foo
add_executable(test_foo test_foo.cpp)
target_include_directories(test_foo PRIVATE include)
target_link_libraries(test_foo digitsynth_static -lgpiod)
add_test(NAME TestFoo COMMAND test_foo)
=======
function(create_test EXE NAME FILE)
    add_executable(${EXE} ${FILE})
    target_include_directories(${EXE} PRIVATE include)
    link_directories("/usr/local/lib/")
    target_link_libraries(${EXE} digitsynth_static gpiod gpiodcxx ads1115rpi rtmidi)
    add_test(NAME ${NAME} COMMAND ${EXE})
endfunction()
>>>>>>> 6e158ed5dc0bb169361d4992cf4e9bc5badc0d83

create_test(test-foo TestFoo test_foo.cpp)
create_test(test-adc-driver TestAdcDriver test_adc-driver.cpp)
create_test(test-button-driver TestButtonDriver test_button-driver.cpp)
# create_test(test-combined TestCombined        synth_logic/combined_test.cpp)
# create_test(test-mode-manager TestModeManager synth_logic/mode_manager_test.cpp)
create_test(test-gpio TestGpio test_gpio.cpp)

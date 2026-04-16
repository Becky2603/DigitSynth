cmake_minimum_required(VERSION 3.23)

enable_testing()

function(create_test EXE NAME FILE)
    add_executable(${EXE} ${FILE})
    target_include_directories(${EXE} PRIVATE include)
    link_directories("/usr/local/lib/")
    target_link_libraries(${EXE} digitsynth_static gpiod gpiodcxx ads1115rpi rtmidi)
    add_test(NAME ${NAME} COMMAND ${EXE})
endfunction()

create_test(test-foo TestFoo test_foo.cpp)
create_test(test-adc-driver TestAdcDriver test_adc-driver.cpp)
create_test(test-combined TestCombined combined_test.cpp)
create_test(test-mode-manager TestModeManager mode_manager_test.cpp)

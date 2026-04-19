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

create_test(scaler-test ScalerTest synth_logic/scaler_test.cpp)
create_test(mode-manager-test ModeManagerTest synth_logic/mode_manager_test.cpp) 
create_test(lfo-manager-test LfoManagerTest synth_logic/lfo_manager_test.cpp)
create_test(led-controller-test LedControllerTest synth_logic/led_controller_test.cpp)
create_test(message-builder-test MessageBuilderTest synth_logic/message_builder_test.cpp)
create_test(chord-manager-test ChordManagerTest synth_logic/chord_manager_test.cpp) 


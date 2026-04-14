cmake_minimum_required(VERSION 3.23)

enable_testing()

# test_foo
add_executable(test_foo test_foo.cpp)
target_include_directories(test_foo PRIVATE include)
target_link_libraries(test_foo digitsynth_static -lgpiod)
add_test(NAME TestFoo COMMAND test_foo)

# test_TLC
add_executable(test_TLC test_TLC.cpp)
target_include_directories(test_TLC PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(test_TLC digitsynth_static)
add_test(NAME TestTLC COMMAND test_TLC)


add_executable(test_combined synth_logic/combined_test.cpp)
target_include_directories(test_TLC PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(test_combined digitsynth_static)
add_test(NAME TestCombined COMMAND test_combined)

add_executable(test_midi_flex synth_logic/midi_flex_test.cpp)
target_include_directories(test_midi_flex PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(test_midi_flex digitsynth_static)
add_test(NAME TestMidiFlex COMMAND test_midi_flex)

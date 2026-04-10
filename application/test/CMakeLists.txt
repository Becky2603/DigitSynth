cmake_minimum_required(VERSION 3.23)

enable_testing()

# test_foo
add_executable(test_foo test_foo.cpp)
add_executable(test_adc-driver test_adc-driver.cpp)
target_include_directories(test_foo PRIVATE include)
target_include_directories(test_adc-driver PRIVATE include)
target_link_libraries(test_foo digitsynth_static -lgpiod)
target_link_libraries(test_adc-driver digitsynth_static -lgpiod)
add_test(NAME TestFoo COMMAND test_foo)

# test_adc-driver
add_executable(test_adc-driver test_adc-driver.cpp)
target_include_directories(test_adc-driver PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(test_adc-driver digitsynth_static)
add_test(NAME TestAdcDriver COMMAND test_adc-driver)

# test_TLC
add_executable(test_TLC test_TLC.cpp)
target_include_directories(test_TLC PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(test_TLC digitsynth_static)
add_test(NAME TestTLC COMMAND test_TLC)

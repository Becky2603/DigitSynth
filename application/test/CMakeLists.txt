cmake_minimum_required(VERSION 3.23)

enable_testing()

add_executable(test_foo test_foo.cpp)
target_include_directories(test_foo PRIVATE include)
target_link_libraries(test_foo digitsynth_static)
add_test(NAME TestFoo COMMAND test_foo)

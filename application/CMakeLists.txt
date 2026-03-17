cmake_minimum_required(VERSION 3.16)
project(tlc59711)
set(CMAKE_CXX_STANDARD 17)

add_executable(tlc59711_hello
    main.cpp
    tlc59711.cpp
)
target_link_libraries(tlc59711_hello lgpio)
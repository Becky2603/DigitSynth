cmake_minimum_required(VERSION 3.23)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)
if (NOT ${RELEASE} EQUAL 0)
    add_compile_options(-O3)
    message(STATUS "Building in release mode ${RELEASE}\n")
else()
    message(STATUS "Building in debug mode ${RELEASE}\n")
    add_compile_options(-O0 -ggdb)
    set(CMAKE_BUILD_TYPE Debug)
endif()

project(DigitSynth)

include(CTest)

set(SOURCE_DIRECTORY src/)

# ── Static library — shared by main binary and tests ────────────────────────
add_library(digitsynth_static STATIC)

target_include_directories(digitsynth_static PUBLIC include)

target_sources(digitsynth_static
    PUBLIC
        ${SOURCE_DIRECTORY}/foo.cpp
        ${SOURCE_DIRECTORY}/adc-driver.cpp
        ${SOURCE_DIRECTORY}/button-driver.cpp
        ${SOURCE_DIRECTORY}/extension-sensor-driver.cpp
        ${SOURCE_DIRECTORY}/TLC59711.cpp      # ← added
)

target_link_libraries(digitsynth_static -lwiringPi -llgpio -lm)

# ── Main executable ──────────────────────────────────────────────────────────
add_executable(digitsynth)

target_include_directories(digitsynth PRIVATE include)

target_sources(digitsynth
    PRIVATE
        ${SOURCE_DIRECTORY}/main.cpp
)

target_link_libraries(digitsynth digitsynth_static)

add_subdirectory(test)cd application
cmake -B build -DRELEASE=0
cmake --build build
cd build && ctest --output-on-failure
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
#include(docs/Docs.cmake)
add_executable(digitsynth)
add_library(digitsynth_static)
target_include_directories(digitsynth PRIVATE include)
target_include_directories(digitsynth_static PUBLIC include)
set(SOURCE_DIRECTORY src/)

target_sources(digitsynth
	PRIVATE
		# Add source files here to build them
		${SOURCE_DIRECTORY}/main.cpp
		${SOURCE_DIRECTORY}/foo.cpp
		${SOURCE_DIRECTORY}/adc-driver.cpp
		${SOURCE_DIRECTORY}/button-driver.cpp
		${SOURCE_DIRECTORY}/gpio.cpp
		${SOURCE_DIRECTORY}/flex-sensor.cpp
		${SOURCE_DIRECTORY}/button-driver.cpp
		${SOURCE_DIRECTORY}/midi-driver.cpp
        ${SOURCE_DIRECTORY}/TLC59711.cpp
		${SOURCE_DIRECTORY}/SynthController.cpp
		${SOURCE_DIRECTORY}/ModeManager.cpp
		${SOURCE_DIRECTORY}/ChordManager.cpp
		${SOURCE_DIRECTORY}/MidiScaler.cpp
		${SOURCE_DIRECTORY}/paramMapper.cpp
        ${SOURCE_DIRECTORY}/patterns.cpp
        ${SOURCE_DIRECTORY}/FlexDSP.cpp
       	${SOURCE_DIRECTORY}/LedController.cpp
)

target_sources(digitsynth_static
	PUBLIC
		# Add source files here to build them
		${SOURCE_DIRECTORY}/foo.cpp
		${SOURCE_DIRECTORY}/adc-driver.cpp
		${SOURCE_DIRECTORY}/button-driver.cpp
		${SOURCE_DIRECTORY}/gpio.cpp
		${SOURCE_DIRECTORY}/midi-driver.cpp
        ${SOURCE_DIRECTORY}/patterns.cpp
        ${SOURCE_DIRECTORY}/TLC59711.cpp
    	${SOURCE_DIRECTORY}/flex-sensor.cpp
    	${SOURCE_DIRECTORY}/button-driver.cpp
    	${SOURCE_DIRECTORY}/SynthController.cpp
    	${SOURCE_DIRECTORY}/ModeManager.cpp
    	${SOURCE_DIRECTORY}/ChordManager.cpp
    	${SOURCE_DIRECTORY}/MidiScaler.cpp
    	${SOURCE_DIRECTORY}/paramMapper.cpp
        ${SOURCE_DIRECTORY}/FlexDSP.cpp
        ${SOURCE_DIRECTORY}/patterns.cpp
        ${SOURCE_DIRECTORY}/TLC59711.cpp
    	${SOURCE_DIRECTORY}/LedController.cpp
)

link_directories("/usr/local/lib/")
link_directories("/lib/aarch64-linux-gnu")

find_library(RTMIDI rtmidi)
find_library(IIR iir)

target_link_libraries(digitsynth        gpiod gpiodcxx ads1115rpi ${RTMIDI} ${IIR})
target_link_libraries(digitsynth_static gpiod gpiodcxx ads1115rpi ${RTMIDI} ${IIR})

add_subdirectory(test)

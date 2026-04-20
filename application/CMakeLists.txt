cmake_minimum_required(VERSION 3.23)

option(BUILD_DOCS "Build documentation with doxygen" OFF)

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

if (BUILD_DOCS)
    message(STATUS "Building docs\n")
    include(docs/Docs.cmake)
    return()
endif (BUILD_DOCS)

add_library(digitsynth_static)
add_executable(digitsynth)
target_include_directories(digitsynth PRIVATE include)
target_include_directories(digitsynth_static PUBLIC include)
set(SOURCE_DIRECTORY src/)

target_sources(digitsynth
	PRIVATE
		${SOURCE_DIRECTORY}/main.cpp
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
        ${SOURCE_DIRECTORY}/tlc59711.cpp
    	${SOURCE_DIRECTORY}/flex-sensor.cpp
    	${SOURCE_DIRECTORY}/button-driver.cpp
    	${SOURCE_DIRECTORY}/synth-controller.cpp
    	${SOURCE_DIRECTORY}/mode-manager.cpp
    	${SOURCE_DIRECTORY}/chord-manager.cpp
    	${SOURCE_DIRECTORY}/midi-scaler.cpp
        ${SOURCE_DIRECTORY}/flex-dsp.cpp
        ${SOURCE_DIRECTORY}/patterns.cpp
        ${SOURCE_DIRECTORY}/tlc59711.cpp
    	${SOURCE_DIRECTORY}/led-controller.cpp
        ${SOURCE_DIRECTORY}/lfo-manager.cpp
        ${SOURCE_DIRECTORY}/message-builder.cpp
        ${SOURCE_DIRECTORY}/voltage-scaler.cpp
)

link_directories("/usr/local/lib/")
link_directories("/lib/aarch64-linux-gnu")

find_library(GPIOD gpiod)
find_library(GPIODCXX gpiodcxx)
find_library(ADS1115 ads1115rpi)
find_library(RTMIDI rtmidi)
find_library(IIR iir)
find_library(ASOUND asound)

target_link_libraries(digitsynth_static 
    ${GPIOD} 
    ${GPIODCXX} 
    ${ADS1115} 
    ${RTMIDI} 
    ${IIR} 
    ${ASOUND}
)
target_link_libraries(digitsynth digitsynth_static)

add_subdirectory(test)

cmake_minimum_required(VERSION 3.23)

# === Compiler options ===
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
		${SOURCE_DIRECTORY}/extension-sensor-driver.cpp
		${SOURCE_DIRECTORY}/spi.cpp
		${SOURCE_DIRECTORY}/gpio.cpp
		${SOURCE_DIRECTORY}/flex-sensor.cpp
)

target_sources(digitsynth_static
	PUBLIC
		# Add source files here to build them
		${SOURCE_DIRECTORY}/foo.cpp
		${SOURCE_DIRECTORY}/adc-driver.cpp
		${SOURCE_DIRECTORY}/button-driver.cpp
		${SOURCE_DIRECTORY}/extension-sensor-driver.cpp
		${SOURCE_DIRECTORY}/spi.cpp
		${SOURCE_DIRECTORY}/gpio.cpp
)

link_directories("/usr/local/lib/")

target_link_libraries(digitsynth -lgpiodcxx libads1115rpi_static.a)
target_link_libraries(digitsynth_static -lgpiodcxx -lads1115rpi)

add_subdirectory(test)

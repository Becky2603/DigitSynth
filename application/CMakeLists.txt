cmake_minimum_required(VERSION 3.23)
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

project(DigitSynth)

add_executable(digitsynth) 
target_include_directories(digitsynth PRIVATE include)
set(SOURCE_DIRECTORY src/)

# Add source files here to build them  
target_sources(digitsynth
	PRIVATE
		${SOURCE_DIRECTORY}/main.cpp 
		${SOURCE_DIRECTORY}/foo.cpp
)

target_link_libraries(digitsynth)

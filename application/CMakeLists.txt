cmake_minimum_required(VERSION 3.23)

# === Compiler options ===
add_compile_options(-Wall -Wextra -Wpedantic -Werror)
if (NOT ${RELEASE} EQUAL 1)
	message(STATUS "Building in debug mode\n")
	add_compile_options(-O0 -g)
else()
	add_compile_options(-O3)
	message(STATUS "Building in release mode\n") 
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
)

target_sources(digitsynth_static
	PUBLIC
		# Add source files here to build them  
		${SOURCE_DIRECTORY}/foo.cpp
)

target_link_libraries(digitsynth)

add_subdirectory(test)

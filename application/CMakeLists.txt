cmake_minimum_required(VERSION 3.23)

project(DigitSynth)

add_executable(digitsynth) 
add_library(foo) 

target_sources(digitsynth
	PRIVATE
		src/main.cpp

	PUBLIC 
		FILE_SET digitsynth_headers
		TYPE HEADERS
		BASE_DIRS 
			include
)

target_sources(foo
	PRIVATE
		src/foo.cpp
	PUBLIC 
		FILE_SET digitsynth_headers
		TYPE HEADERS
		BASE_DIRS 
			include
)

target_link_libraries(digitsynth foo)

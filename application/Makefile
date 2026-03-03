
.PHONY: all run clean test

all: 
	cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=On 
	cmake --build build/

run: all
	./build/digitsynth

test: all
	cmake --build build/ -- test

clean: 
	cmake --build build/ -- clean 

GNUMAKEFLAGS := j4

.PHONY: all run clean test docs

all: 
	cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=On 
	cmake --build build/

run: all
	./build/digitsynth

test: all
	cmake --build build/ -- test

docs: all
	sudo cp -r build/html/* /data/www/
	sudo nginx -s reload
	

clean: 
	cmake --build build/ -- clean

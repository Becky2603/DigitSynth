
PACKAGENAME=digitsynth-arm64

.PHONY: all run clean test docs deb

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
	
deb: all
	./create-package.sh


clean: 
	cmake --build build/ -- clean

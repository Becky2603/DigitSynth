NPROC=$(shell nproc --all)

PACKAGENAME=digitsynth-arm64

.PHONY: all run clean test docs deb docs-web

all: 
	cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=On -DBUILD_DOCS=OFF
	cmake --build build/  --parallel $(NPROC)

run: all
	./build/digitsynth

test: all
	-cmake --build build/ -- test > ./build/tests.log
	-cat ./build/tests.log
	-cat ./build/Testing/Temporary/LastTest.log
	tail -n 5 ./build/tests.log
	

docs: 
	cmake . -B build/ -DBUILD_DOCS=ON
	cmake --build build/  --parallel $(NPROC)
	tar -czvf build/digitsynth-documentation.tar.gz build/html/
	zip -r    build/digitsynth-documentation.zip    build/html/ 
		
deb: all
	./create-package.sh

docs-web: docs
	sudo cp -r build/html/* /data/www/
	sudo nginx -s reload
	
clean: 
	cmake --build build/ -- clean

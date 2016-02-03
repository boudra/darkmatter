all: ./build/Makefile
	make -C build

./build/Makefile:
	mkdir -p build
	cd build && cmake ..

clean:
	rm -rf build/*

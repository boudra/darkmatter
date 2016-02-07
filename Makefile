all: build/Makefile
	make -C build

build/Makefile:
	mkdir -p build
	cd build && cmake ..

test:
	mkdir -p build
	cd build && cmake .. -DENABLE_TESTS=1
	make -C build
	for test in tests/*; do ./$$test; done

clean:
	rm -rf build/*
	rm -rf tests/*

format:
	find src/**/*.[hc]pp | xargs clang-format -i -style=file

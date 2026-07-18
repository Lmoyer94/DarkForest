.PHONY: all
all:
	g++ src/main.cpp src/civ.cpp -o bin/release/darkforest
	g++ src/main.cpp src/civ.cpp -g -o bin/debug/darkforest
	./bin/release/darkforest

.PHONY: build
build:
	g++ src/main.cpp src/civ.cpp -o bin/release/darkforest
	g++ src/main.cpp src/civ.cpp -g -o bin/debug/darkforest

.PHONY: run
run:
	./bin/release/darkforest 1000 100000
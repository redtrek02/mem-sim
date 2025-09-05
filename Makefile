CC := gcc
SHELL := /bin/bash

build/program: build/main.o build/mem_sim.o build/commands.o
	$(CC) build/main.o build/mem_sim.o build/commands.o -o build/program 

build/main.o: src/main.c src/mem_sim.h src/commands.h
	$(CC) -c src/main.c -o build/main.o

build/mem_sim.o: src/mem_sim.c src/mem_sim.h src/commands.h
	$(CC) -c src/mem_sim.c -o build/mem_sim.o

build/commands.o: src/commands.c src/commands.h src/mem_sim.h
	$(CC) -c src/commands.c -o build/commands.o

.PHONY: clean test
clean:
	rm -f build/*.o build/program

test: build/program
	pushd build && ./program && popd
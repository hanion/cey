CC = gcc
CFLAGS = -Wall -Werror -g3

.PHONY: all clean

all: build/cey build/yec bootstrap

clean:
	rm -rf build

build:
	mkdir -p build

build/cey: src/cey.c | build
	$(CC) $(CFLAGS) -o build/cey src/cey.c

build/yec: src/yec.c | build
	$(CC) $(CFLAGS) -o build/yec src/yec.c

build/amalgamator: src/amalgamator.c | build
	$(CC) $(CFLAGS) -o build/amalgamator src/amalgamator.c


bootstrap: build build/amalgamator build/yec build/cey
	@echo "bootstrapping..."
	build/amalgamator src/cey.c -I src -o build/amalgamation.c
	rm -f build/cey.cy
	build/yec build/amalgamation.c build/cey.cy
	build/cey  $(CFLAGS) -o build/bcey build/cey.cy
	build/bcey $(CFLAGS) -o build/bcey build/cey.cy


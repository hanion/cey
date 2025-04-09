CC = gcc
CFLAGS = -Wall -Werror -g3

BUILD = build/

all: cey

cey: src/cey.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -o $(BUILD)cey src/cey.c


clean:
	rm -rf $(BUILD)

test: clean cey
	$(BUILD)cey $(CFLAGS) -o build/main src/main.cy -- --cc=clang

.PHONY: all clean test

CC = gcc
CFLAGS = -Wall -Werror

SRC = ./src/
BUILD = ./build/

all: main

main: $(SRC)main.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -o $(BUILD)main $(SRC)main.c

cey: cey.c
	$(CC) $(CFLAGS) -o $(BUILD)cey $(SRC)cey.c

yec: yec.c
	$(CC) $(CFLAGS) -o $(BUILD)yec $(SRC)yec.c


clean:
	rm -rf $(BUILD)

run: clean main
	$(BUILD)main

compile: clean main
	$(BUILD)main
	gcc -o $(BUILD)result $(BUILD)result.c
	$(BUILD)result

.PHONY: all clean run compile

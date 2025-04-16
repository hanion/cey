CC = gcc
CFLAGS = -Wall -Werror -g3

BUILD = build/

all: cey yec

cey: src/cey.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -o $(BUILD)cey src/cey.c

yec: src/yec.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -o $(BUILD)yec src/yec.c

clean:
	rm -rf $(BUILD)

bootstrap: cey
	$(BUILD)cey $(CFLAGS) -o build/cey ./examples/cey.cy

.PHONY: all clean bootstrap

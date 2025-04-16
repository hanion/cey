CC = gcc
CFLAGS = -Wall -Werror -g3

BUILD = build/

all: cey

cey: src/cey.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -o $(BUILD)cey src/cey.c


clean:
	rm -rf $(BUILD)

bootstrap: cey
	$(BUILD)cey $(CFLAGS) -o build/cey ./examples/cey.cy

.PHONY: all clean bootstrap

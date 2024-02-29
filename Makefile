all:
	$(CC) main.c -O3 -Wall -lm $(shell pkg-config --libs --cflags sdl2)  -o fishtude

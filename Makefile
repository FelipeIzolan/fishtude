all:
	$(CC) main.c -Wall -lm $(shell pkg-config --libs --cflags sdl2)  -o fishtude

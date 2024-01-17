all:
	$(CC) main.c -lm $(shell pkg-config --libs --cflags sdl2)  -o fishtude

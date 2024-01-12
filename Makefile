all:
	$(CC) main.c $(shell pkg-config --libs --cflags sdl2)  -o fishtude

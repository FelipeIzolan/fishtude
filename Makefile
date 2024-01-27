all:
	$(CC) main.c -fms-extensions -lm $(shell pkg-config --libs --cflags sdl2)  -o fishtude

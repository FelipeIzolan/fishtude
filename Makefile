CC = clang

all: texture_header compile

compile:
	$(CC) main.c -Wall -O3 -lm $(shell pkg-config --libs --cflags sdl2) -o fishtude

texture_header:
	cd ./script; python generateTextureHeader.py


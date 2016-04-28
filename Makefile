CC=gcc
CFLAGS=-lSDL -lSDL_mixer -lvorbis -lSDL_gfx -lSDL_ttf

bossintro: bossintro.o
	$(CC) -o bossintro bossintro.o $(CFLAGS)

clean:
	rm bossintro bossintro.o

all: bossintro

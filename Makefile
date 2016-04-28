CC=gcc
CFLAGS=-I.
LDFLAGS=-lSDL -lSDL_mixer -lvorbis -lSDL_gfx
bossintro: bossintro.o
	$(CC) -o bossintro bossintro.o $(CFLAGS) $(LDFLAGS)

clean:
	rm bossintro bossintro.o

all: bossintro

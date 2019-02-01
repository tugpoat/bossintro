CC=gcc
CFLAGS=-I. -DWINDOWED
LDFLAGS=-lstdc++ -lSDL -lSDL_mixer -lvorbis -lSDL_gfx -lSDL_ttf
bossintro: bossintro.o
	$(CC) -o bossintro bossintro.o $(CFLAGS) $(LDFLAGS)

clean:
	rm bossintro bossintro.o

all: bossintro

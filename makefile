CFLAGS = -Wall -pedantic -Werror -lm -g
LFLAGS = `sdl-config --libs` -lSDL

all: main

main:
	gcc asteroids.c -o asteroids.out $(CFLAGS) $(LFLAGS)
	
clean:
	rm asteroids

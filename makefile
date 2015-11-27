CFLAGS = -Wall -pedantic -Werror -lm -g
LFLAGS = `sdl-config --libs` -lSDL

all: main

main:
	gcc asteroids.c -o asteroids $(CFLAGS) $(LFLAGS)
	
clean:
	rm asteroids
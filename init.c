#include "asteroids.h"

void cleanup()
{
	SDL_Quit();
	exit(0);
}

void init_shapes(SDL_Surface* screen) {
	pixel_t* pxl_array = (pixel_t*) calloc(5, sizeof(pixel_t));
	pxl_array[0].x = 20; pxl_array[0].y = 0; 
	pxl_array[1].x = -4; pxl_array[1].y = 10; 
	pxl_array[2].x = 0; pxl_array[2].y = 8; 
	pxl_array[3].x = 0; pxl_array[3].y = -8; 
	pxl_array[4].x = -4; pxl_array[4].y = -10;
	SHIP_DRAWING_SHAPE.nodes = pxl_array;
	SHIP_DRAWING_SHAPE.count = 5;
	SHIP_DRAWING_SHAPE.color = SDL_MapRGB(screen -> format, 0xff, 0xff, 0xff);
	
	pxl_array = (pixel_t*) calloc(3, sizeof(pixel_t));
	pxl_array[0].x = -1; pxl_array[0].y = 4; 
	pxl_array[1].x = -6; pxl_array[1].y = 0; 
	pxl_array[2].x = -1; pxl_array[2].y = -4; 
	ENGINE_DRAWING_SHAPE.nodes = pxl_array;
	ENGINE_DRAWING_SHAPE.count = 3;
	ENGINE_DRAWING_SHAPE.color = SDL_MapRGB(screen -> format, 0xff, 0, 0);
	
	pxl_array = (pixel_t*) calloc(3, sizeof(pixel_t));
	pxl_array[0].x = 20; pxl_array[0].y = 0; 
	pxl_array[1].x = 0; pxl_array[1].y = 8; 
	pxl_array[2].x = 0; pxl_array[2].y = -8; 
	SHIP_COLLISION_SHAPE.nodes = pxl_array;
	SHIP_COLLISION_SHAPE.count = 3;
	SHIP_COLLISION_SHAPE.color = 0;
	
	pxl_array = (pixel_t*) calloc(10, sizeof(pixel_t));
	pxl_array[0].x = -2; pxl_array[0].y = 0; 
	pxl_array[1].x = -5; pxl_array[1].y = 2; 
	pxl_array[2].x = 1; pxl_array[2].y = 5; 
	pxl_array[3].x = 3; pxl_array[3].y = 4; 
	pxl_array[4].x = 1; pxl_array[4].y = -1; 
	pxl_array[5].x = 4; pxl_array[5].y = 1; 
	pxl_array[6].x = 5; pxl_array[6].y = -3; 
	pxl_array[7].x = 3; pxl_array[7].y = -5; 
	pxl_array[8].x = -3; pxl_array[8].y = -5; 
	pxl_array[9].x = -5; pxl_array[9].y = -1; 
	ASTEROID_DRAWING_SHAPE.nodes = pxl_array;
	ASTEROID_DRAWING_SHAPE.count = 10;
	ASTEROID_DRAWING_SHAPE.color = SDL_MapRGB(screen -> format, 0xff, 0xff, 0xff);
	
	pxl_array = (pixel_t*) calloc(4, sizeof(pixel_t));
	pxl_array[0].x = -4; pxl_array[0].y = -4; 
	pxl_array[1].x = -4; pxl_array[1].y = 4; 
	pxl_array[2].x = 4; pxl_array[2].y = 4; 
	pxl_array[3].x = 4; pxl_array[3].y = -4;
	ASTEROID_COLLISION_SHAPE.nodes = pxl_array;
	ASTEROID_COLLISION_SHAPE.count = 4;
	ASTEROID_COLLISION_SHAPE.color = 0;
}

SDL_Surface* init_video()
{
	SDL_Surface* screen;
    if((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)==-1)) { 
        fprintf(stderr, "Could not initialize SDL: %s.\n",
						SDL_GetError());
        exit(-1);
    }
	atexit(cleanup); 
	screen = SDL_SetVideoMode(WIDTH, HEIGHT, 8, SDL_SWSURFACE);
    if ( screen == NULL ) {
        fprintf(stderr, "Couldn't set %dx%dx8 video mode: %s\n",
						WIDTH, HEIGHT,
                        SDL_GetError());
        exit(1);
    }
	SDL_WM_SetCaption("asteroids", NULL);
	
	init_shapes(screen);
	
	return screen;
}

ship_t* init_ship()
{	
	ship_t* ship  = (ship_t*) malloc(sizeof(ship_t));
	ship -> x = WIDTH / 2;
	ship -> y = HEIGHT / 2;
	ship -> speed_x = 0;
	ship -> speed_y = 0;
	ship -> rotating_left = 0;
	ship -> rotating_right = 0;
	ship -> engine = 0;
	ship -> angle = M_PI / 2;
	return ship;
}

asteroid_t* init_asteroids()
{
	asteroid_t* ast = (asteroid_t*) malloc(sizeof(asteroid_t));
	ast -> x = 200;
	ast -> y = 200;
	ast -> speed_x = 0;
	ast -> speed_y = 0;
	ast -> size = 3;
	ast -> next = NULL;
	return ast;
}

world_t* init_world()
{
	world_t* world = (world_t*) malloc(sizeof(world_t));
	world -> ship = init_ship();
	world -> asteroids = init_asteroids();
	world -> missles = NULL;
	world -> enemies = NULL;
	return world;
}
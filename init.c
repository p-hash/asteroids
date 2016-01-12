#include "asteroids.h"

void free_shape(shape_t* shape)
{
	if (shape != NULL)
	{
		free(shape -> nodes);
		free(shape);
	}
}	

void free_world(world_t* world)
{
	asteroid_t* ast = world -> asteroids;
	missle_t* msl = world -> missles;
	while (msl != NULL)
	{
		missle_t* tmp = msl -> next;
		free(msl);
		msl = tmp;
	}
	while (ast != NULL)
	{
		asteroid_t* tmp = ast -> next;
		free(ast);
		ast = tmp;
	}
	free(world -> ship);
}

void free_digits(shape_t* digits)
{
	int i; 
	for (i = 0; i < 10; i++)
		free(digits[i].nodes);
	free(digits);
}

void cleanup()
{
	free_shape(SHIP_COLLISION_SHAPE);
	free_shape(SHIP_DRAWING_SHAPE);
	free_shape(ENGINE_DRAWING_SHAPE);
	free_shape(ASTEROID_COLLISION_SHAPE);
	free_shape(ASTEROID_DRAWING_SHAPE);
	free_shape(MISSLE_SHAPE);
	free_digits(DIGITS_SHAPES);
	SDL_Quit();
	exit(0);
}

void init_digit_shapes(SDL_Surface* screen)
{
	pixel_t* pxl_array;
	shape_t* shape;
	int i;
	uint32_t color = SDL_MapRGB(screen -> format, 0xff, 0xff, 0xff);
	DIGITS_SHAPES = (shape_t*) malloc(10 * sizeof(shape_t));
	for (i = 0; i < 10; i++)
		DIGITS_SHAPES[i].color = color;
	
	shape = DIGITS_SHAPES;	
	pxl_array = (pixel_t*) malloc(6 * sizeof(pixel_t)); /* 0 */
	pxl_array[0].x = 5; pxl_array[0].y = -10; 
	pxl_array[1].x = 5; pxl_array[1].y = 10; 
	pxl_array[2].x = -5; pxl_array[2].y = 10; 
	pxl_array[3].x = -5; pxl_array[3].y = -10; 
	pxl_array[4].x = 5; pxl_array[4].y = -10; 
	pxl_array[5].x = -5; pxl_array[5].y = 10; 
	shape -> nodes = pxl_array;
	shape -> count = 6;
	shape -> is_poligon = 0;
	shape++;
	
	pxl_array = (pixel_t*) malloc(5 * sizeof(pixel_t)); /* 1 */
	pxl_array[0].x = -5; pxl_array[0].y = -10; 
	pxl_array[1].x = 0; pxl_array[1].y = -10; 
	pxl_array[2].x = 0; pxl_array[2].y = 10; 
	pxl_array[3].x = 5; pxl_array[3].y = 10; 
	pxl_array[4].x = -5; pxl_array[4].y = 10;
	shape -> nodes = pxl_array;
	shape -> count = 5;
	shape -> is_poligon = 0;
	shape++;
	
	pxl_array = (pixel_t*) malloc(5 * sizeof(pixel_t)); /* 2 */
	pxl_array[0].x = -5; pxl_array[0].y = -10; 
	pxl_array[1].x = 5; pxl_array[1].y = -10; 
	pxl_array[2].x = 5; pxl_array[2].y = 0; 
	pxl_array[3].x = -5; pxl_array[3].y = 10; 
	pxl_array[4].x = 5; pxl_array[4].y = 10;
	shape -> nodes = pxl_array;
	shape -> count = 5;
	shape -> is_poligon = 0;
	shape++;
	
	pxl_array = (pixel_t*) malloc(5 * sizeof(pixel_t)); /* 3 */
	pxl_array[0].x = -5; pxl_array[0].y = -10; 
	pxl_array[1].x = 5; pxl_array[1].y = -10; 
	pxl_array[2].x = -5; pxl_array[2].y = 0; 
	pxl_array[3].x = 5; pxl_array[3].y = 0; 
	pxl_array[4].x = -5; pxl_array[4].y = 10;
	shape -> nodes = pxl_array;
	shape -> count = 5;
	shape -> is_poligon = 0;
	shape++;
	
	pxl_array = (pixel_t*) malloc(5 * sizeof(pixel_t)); /* 4 */
	pxl_array[0].x = -5; pxl_array[0].y = -10; 
	pxl_array[1].x = -5; pxl_array[1].y = 0; 
	pxl_array[2].x = 5; pxl_array[2].y = 0; 
	pxl_array[3].x = 5; pxl_array[3].y = -10; 
	pxl_array[4].x = 5; pxl_array[4].y = 10;
	shape -> nodes = pxl_array;
	shape -> count = 5;
	shape -> is_poligon = 0;
	shape++;
	
	pxl_array = (pixel_t*) malloc(6 * sizeof(pixel_t)); /* 5 */
	pxl_array[0].x = 5; pxl_array[0].y = -10; 
	pxl_array[1].x = -5; pxl_array[1].y = -10; 
	pxl_array[2].x = -5; pxl_array[2].y = 0; 
	pxl_array[3].x = 5; pxl_array[3].y = 0; 
	pxl_array[4].x = 5; pxl_array[4].y = 10;
	pxl_array[5].x = -5; pxl_array[5].y = 10;
	shape -> nodes = pxl_array;
	shape -> count = 6;
	shape -> is_poligon = 0;
	shape++;
	
	pxl_array = (pixel_t*) malloc(6 * sizeof(pixel_t)); /* 6 */
	pxl_array[0].x = 5; pxl_array[0].y = -10; 
	pxl_array[1].x = -5; pxl_array[1].y = -10; 
	pxl_array[2].x = -5; pxl_array[2].y = 10; 
	pxl_array[3].x = 5; pxl_array[3].y = 10; 
	pxl_array[4].x = 5; pxl_array[4].y = 0;
	pxl_array[5].x = -5; pxl_array[5].y = 0;
	shape -> nodes = pxl_array;
	shape -> count = 6;
	shape -> is_poligon = 0;
	shape++;
	
	pxl_array = (pixel_t*) malloc(4 * sizeof(pixel_t)); /* 7 */
	pxl_array[0].x = -5; pxl_array[0].y = -10; 
	pxl_array[1].x = 5; pxl_array[1].y = -10; 
	pxl_array[2].x = 0; pxl_array[2].y = 0; 
	pxl_array[3].x = 0; pxl_array[3].y = 10; 
	shape -> nodes = pxl_array;
	shape -> count = 4;
	shape -> is_poligon = 0;
	shape++;
	
	pxl_array = (pixel_t*) malloc(7 * sizeof(pixel_t)); /* 8 */
	pxl_array[0].x = 5; pxl_array[0].y = 0; 
	pxl_array[1].x = -5; pxl_array[1].y = 0; 
	pxl_array[2].x = -5; pxl_array[2].y = 10; 
	pxl_array[3].x = 5; pxl_array[3].y = 10; 
	pxl_array[4].x = 5; pxl_array[4].y = -10;
	pxl_array[5].x = -5; pxl_array[5].y = -10;
	pxl_array[6].x = -5; pxl_array[6].y = 0;
	shape -> nodes = pxl_array;
	shape -> count = 7;
	shape -> is_poligon = 0;
	shape++;
	
	pxl_array = (pixel_t*) malloc(6 * sizeof(pixel_t)); /* 9 */
	pxl_array[0].x = 5; pxl_array[0].y = 0; 
	pxl_array[1].x = -5; pxl_array[1].y = 0; 
	pxl_array[2].x = -5; pxl_array[2].y = -10; 
	pxl_array[3].x = 5; pxl_array[3].y = -10; 
	pxl_array[4].x = 5; pxl_array[4].y = 0;
	pxl_array[5].x = 5; pxl_array[5].y = 10;
	shape -> nodes = pxl_array;
	shape -> count = 6;
	shape -> is_poligon = 0;
}

void init_shapes(SDL_Surface* screen) {
	pixel_t* pxl_array;
	
	SHIP_DRAWING_SHAPE = (shape_t*) malloc(sizeof(shape_t));
	pxl_array = (pixel_t*) malloc(5 * sizeof(pixel_t));
	pxl_array[0].x = 0; pxl_array[0].y = -20; 
	pxl_array[1].x = 10; pxl_array[1].y = 4; 
	pxl_array[2].x = 8; pxl_array[2].y = 0; 
	pxl_array[3].x = -8; pxl_array[3].y = 0; 
	pxl_array[4].x = -10; pxl_array[4].y = 4;
	SHIP_DRAWING_SHAPE -> nodes = pxl_array;
	SHIP_DRAWING_SHAPE -> count = 5;
	SHIP_DRAWING_SHAPE -> color = SDL_MapRGB(screen -> format, 0xff, 0xff, 0xff);
	SHIP_DRAWING_SHAPE -> is_poligon = 1;
	
	ENGINE_DRAWING_SHAPE = (shape_t*) malloc(sizeof(shape_t));
	pxl_array = (pixel_t*) malloc(3 * sizeof(pixel_t));
	pxl_array[0].x = 4; pxl_array[0].y = 1; 
	pxl_array[1].x = 0; pxl_array[1].y = 6; 
	pxl_array[2].x = -4; pxl_array[2].y = 1; 
	ENGINE_DRAWING_SHAPE -> nodes = pxl_array;
	ENGINE_DRAWING_SHAPE -> count = 3;
	ENGINE_DRAWING_SHAPE -> color = SDL_MapRGB(screen -> format, 0xff, 0, 0);
	ENGINE_DRAWING_SHAPE -> is_poligon = 0;
	
	SHIP_COLLISION_SHAPE = (shape_t*) malloc(sizeof(shape_t));
	pxl_array = (pixel_t*) malloc(3 * sizeof(pixel_t));
	pxl_array[0].x = 0; pxl_array[0].y = -20; 
	pxl_array[1].x = 8; pxl_array[1].y = 0; 
	pxl_array[2].x = -8; pxl_array[2].y = 0; 
	SHIP_COLLISION_SHAPE -> nodes = pxl_array;
	SHIP_COLLISION_SHAPE -> count = 3;
	SHIP_COLLISION_SHAPE -> color = 0;
	SHIP_COLLISION_SHAPE -> is_poligon = 1;
	
	ASTEROID_DRAWING_SHAPE = (shape_t*) malloc(sizeof(shape_t));
	pxl_array = (pixel_t*) malloc(10 * sizeof(pixel_t));
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
	ASTEROID_DRAWING_SHAPE -> nodes = pxl_array;
	ASTEROID_DRAWING_SHAPE -> count = 10;
	ASTEROID_DRAWING_SHAPE -> color = SDL_MapRGB(screen -> format, 0xff, 0xff, 0xff);
	ASTEROID_DRAWING_SHAPE -> is_poligon = 1;
	
	ASTEROID_COLLISION_SHAPE = (shape_t*) malloc(sizeof(shape_t));
	pxl_array = (pixel_t*) malloc(4 * sizeof(pixel_t));
	pxl_array[0].x = -4; pxl_array[0].y = -4; 
	pxl_array[1].x = -4; pxl_array[1].y = 4; 
	pxl_array[2].x = 4; pxl_array[2].y = 4; 
	pxl_array[3].x = 4; pxl_array[3].y = -4;
	ASTEROID_COLLISION_SHAPE -> nodes = pxl_array;
	ASTEROID_COLLISION_SHAPE -> count = 4;
	ASTEROID_COLLISION_SHAPE -> color = 0;
	ASTEROID_COLLISION_SHAPE -> is_poligon = 1;
	
	MISSLE_SHAPE = (shape_t*) malloc(sizeof(shape_t));
	pxl_array = (pixel_t*) malloc(5 * sizeof(pixel_t));
	pxl_array[0].x = -1; pxl_array[0].y = 0; 
	pxl_array[1].x = 1; pxl_array[1].y = 0; 
	pxl_array[2].x = 0; pxl_array[2].y = 0; 
	pxl_array[3].x = 0; pxl_array[3].y = -1;
	pxl_array[4].x = 0; pxl_array[4].y = 1;
	MISSLE_SHAPE -> nodes = pxl_array;
	MISSLE_SHAPE -> count = 5;
	MISSLE_SHAPE -> color = SDL_MapRGB(screen -> format, 0xff, 0xff, 0x00);	
	MISSLE_SHAPE -> is_poligon = 0;
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
	init_digit_shapes(screen);
	
	return screen;
}

ship_t* init_ship(int lifes)
{	
	ship_t* ship  = (ship_t*) malloc(sizeof(ship_t));
	ship -> x = WIDTH / 2;
	ship -> y = HEIGHT / 2;
	ship -> speed_x = 0;
	ship -> speed_y = 0;
	ship -> rotating_left = 0;
	ship -> rotating_right = 0;
	ship -> engine = 0;
	ship -> angle = 0;
	ship -> shoot = 0;
	ship -> lifes = lifes;
	return ship;
}

asteroid_t* init_asteroids()
{
	int count = rand() % 3 + 1;
	int i;
	asteroid_t* ast = NULL;
	for (i = 0; i < count; i++)
	{
		asteroid_t* new_ast = (asteroid_t*) malloc(sizeof(asteroid_t));
		if (rand() % 2 == 0) {
			new_ast -> x = rand() % WIDTH;
			new_ast -> y = 1;
		}
		else
		{
			new_ast -> x = 1;
			new_ast -> y = rand() % HEIGHT;
		}
		new_ast -> speed_x = rand() % ASTEROID_MIN_SPEED;
		new_ast -> speed_y = rand() % ASTEROID_MIN_SPEED;
		new_ast -> size = 3;
		new_ast -> next = ast;
		ast = new_ast;
	}
	return ast;
}

world_t* init_world()
{
	world_t* world = (world_t*) malloc(sizeof(world_t));
	world -> ship = init_ship(3);
	world -> asteroids = init_asteroids();
	world -> missles = NULL;
	world -> enemies = NULL;
	world -> score = 0;
	return world;
}
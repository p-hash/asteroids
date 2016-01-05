#include <SDL/SDL.h>
#include <stdio.h>
#include <math.h>

#define EPS 0.000001
#define WIDTH 640
#define HEIGHT 480

#define SHIP_MAX_SPEED 20
#define MISSLE_SPEED 13
#define MISSLE_LIFE 30

typedef struct {
	int x;
	int y;
} pixel_t;

typedef struct {
	double x;
	double y;
	double angle;
	int engine;
	int rotating_right;
	int rotating_left;
	int shoot;
	double speed_x;
	double speed_y;
} ship_t;

typedef struct missle_t {
	double x;
	double y;
	double speed_x;
	double speed_y;	
	int life;
	struct missle_t* next;
} missle_t;

typedef struct asteroid_t {
	double x;
	double y;
	double speed_x;
	double speed_y;
	int size;
	struct asteroid_t* next;
} asteroid_t;

typedef struct {
	double x;
	double y;
	double speed_x;
	double speed_y;	
} enemy_t;

typedef struct {
	ship_t* ship;
	missle_t* missles;
	asteroid_t* asteroids;
	enemy_t* enemies;
} world_t;

void cleanup()
{
	SDL_Quit();
	exit(0);
}

void putpixel(SDL_Surface *screen, pixel_t* pixel, uint32_t color)
{
    int bpp = screen->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    uint8_t *p = (uint8_t *) screen -> pixels + pixel -> y * screen -> pitch + pixel -> x * bpp;	
	if (pixel -> y < 0)
		return;
	if (pixel -> x < 0)
		return;
	
    switch(bpp) {
    case 1:
        *p = color;
        break;

    case 2:
        *(uint16_t *)p = color;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (color >> 16) & 0xff;
            p[1] = (color >> 8) & 0xff;
            p[2] = color & 0xff;
        } else {
            p[0] = color & 0xff;
            p[1] = (color >> 8) & 0xff;
            p[2] = (color >> 16) & 0xff;
        }
        break;

    case 4:
        *(uint32_t *)p = color;
        break;
    }
}

pixel_t* pixel_create(unsigned int const x, 
					   unsigned int const y)
{
	pixel_t* pixel = (pixel_t*) malloc(sizeof(pixel_t));
	/* Map the color yellow to this display (R=0xff, G=0xFF, B=0x00)
	   Note:  If the display is palettized, you must set the palette first.
	*/
	pixel -> x = x;
	pixel -> y = y;
	return pixel;
}

void swap(int* a, int* b) 
{
	int r = *a;
	*a = *b;
	*b = r;
}

void line(SDL_Surface* surface, pixel_t* const from, pixel_t* const to, uint32_t color)
{	
	double dy = (to -> y) - (from -> y);
	double dx = (to -> x) - (from -> x);
	if (abs(dx) >= abs(dy))
	{
		int x;
		int a = from -> x;
		int b = to -> x;
		double k = dy / dx;
		double c = from -> y + 0.5 - k * (from -> x);
		if (b < a) swap(&a, &b);
		for (x = a; x <= b; x++)
		{
			int y = (int) floor( k * x + c );
			putpixel(surface, pixel_create(x, y), color);
		}
	}
	else
	{
		int y;
		int a = from -> y;
		int b = to -> y;
		double k = dx / dy;
		double c = from -> x + 0.5 - k * (from -> y);
		if (b < a) swap(&a, &b);
		for (y = a; y <= b; y++)
		{
			int x = (int) floor( k * y + c );
			putpixel(surface, pixel_create(x, y), color);
		}
	}
}

pixel_t* pixel_create_from(pixel_t* o, int dx, int dy, double angle)
{
	int x = (int) floor(dx * cos(angle) + dy * sin(angle));
	int y = (int) floor( -1 * dx * sin(angle) + dy * cos(angle));
	x += o -> x;
	y += o -> y;
	return pixel_create(x, y);
}

void draw_ship(SDL_Surface* screen, ship_t* ship)
{ /* ДАВАЙ РИСОВАТЬ КОРАБЛИК! */
	pixel_t* center = pixel_create((int)round(ship->x),(int)round(ship->y));
	double angle = ship -> angle;
	uint32_t color = SDL_MapRGB(screen->format, 0xff, 0xff, 0xff);
	pixel_t* front = pixel_create_from(center, 20, 0, angle);
	pixel_t* right = pixel_create_from(center, 0, 8, angle);
	pixel_t* right_back = pixel_create_from(center, -4, 10, angle);
	pixel_t* left = pixel_create_from(center, 0, -8, angle);
	pixel_t* left_back = pixel_create_from(center, -4, -10, angle);
	line(screen, front, right_back, color);
	line(screen, front, left_back, color);
	line(screen, left, right, color);
	free(front);
	free(right); free(right_back);
	free(left); free(left_back);
}

void draw_asts(SDL_Surface* screen, asteroid_t* list)
{
	
}

void draw_missles(SDL_Surface* screen, missle_t* list)
{
	uint32_t color = SDL_MapRGB(screen->format, 0xff, 0xff, 0x00);
	while (list != NULL)
	{
		pixel_t* pixel = pixel_create((int)list -> x, (int)list -> y);
		putpixel(screen, pixel, color);
		free(pixel);
		list = list -> next;
	}
}

void draw(SDL_Surface* screen, world_t* world)
{
	if ( SDL_MUSTLOCK(screen) ) {
		if ( SDL_LockSurface(screen) < 0 ) {
			fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
		}
	}
	SDL_FillRect(screen, NULL, 0x000000);
	
	draw_ship(screen, world -> ship);
	draw_asts(screen, world -> asteroids);
	draw_missles(screen, world -> missles);
	
	if ( SDL_MUSTLOCK(screen) ) {
		SDL_UnlockSurface(screen);
	}
	SDL_UpdateRect(screen, 0, 0, 0, 0);
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
	
	return screen;
}

ship_t* init_ship()
{	
	ship_t* ship  = (ship_t*) malloc(sizeof(ship_t));
	ship -> x = 70;
	ship -> y = 70;
	ship -> speed_x = 0;
	ship -> speed_y = 0;
	ship -> rotating_left = 0;
	ship -> rotating_right = 0;
	ship -> engine = 0;
	ship -> angle = 0;
	return ship;
}

asteroid_t* init_asteroids()
{
	return NULL;
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

void process_input(ship_t* ship)
{
	SDL_Event event;	
	while(SDL_PollEvent(&event))
	{  
		switch(event.type)
		{  				
			case SDL_KEYDOWN:
			/* handle keyboard stuff here */
				switch (event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						exit(0);
					break;
					
					case SDLK_LEFT:
						ship -> rotating_left = 1;
					break;
					
					case SDLK_RIGHT:
						ship ->rotating_right = 1;
					break;
					
					case SDLK_UP:
						ship -> engine = 1;
					break;
					
					case SDLK_SPACE:
						ship -> shoot = 1;
					break;
					
					default:
					break;
				}				
			break;
		
			case SDL_KEYUP:
				switch (event.key.keysym.sym)
				{
					case SDLK_LEFT:
						ship -> rotating_left = 0;
					break;
					
					case SDLK_RIGHT:
						ship -> rotating_right = 0;
					break;
					
					case SDLK_UP:
						ship -> engine = 0;
					break;
					
					default:
					break;
				}
			break;

			case SDL_QUIT:
			/* Set whatever flags are necessary to */
			/* end the main game loop here */
				exit(0);
			break;
		}
	}
}

double sqr(double x)
{
	return x*x;
}

void update_ship(ship_t* ship)
{
	double degree = 0.08; 
	if (ship -> rotating_left) 
	{
		ship -> angle += degree;
	}
	if (ship -> rotating_right)
	{
		ship -> angle -= degree;
	}
	
	if (ship -> engine)
	{
		if ( (sqr(ship -> speed_x) + sqr(ship -> speed_y)) < SHIP_MAX_SPEED ) 
		{
			double accel = 0.075;
			ship -> speed_x += accel * cos(ship -> angle);
			ship -> speed_y -= accel * sin(ship -> angle);
		}
	}
	else
	{
		double accel = 0.995;
		ship -> speed_x *= accel;
		ship -> speed_y *= accel;
	}	
		
	ship -> x += ship -> speed_x;
	ship -> y += ship -> speed_y;
	if (ship -> x > WIDTH) ship -> x -= WIDTH;
	if (ship -> x < 0) ship -> x += WIDTH;
	if (ship -> y > HEIGHT) ship -> y -= HEIGHT;
	if (ship -> y < 0) ship -> y += HEIGHT;
}

asteroid_t* update_asts(asteroid_t* list) {
	asteroid_t* ret = list;
	asteroid_t* ast = list;
	while (ast != NULL)
	{
		ast -> x += ast -> speed_x;
		ast -> y += ast -> speed_y;
		ast = ast -> next;
	}
	return ret;
}

missle_t* add_missle(missle_t* list, ship_t* ship) 
{
	missle_t* missle;
	double _cos = cos(ship -> angle);
	double _sin = sin(ship -> angle);
	if (!(ship -> shoot)) { return list; }
	missle = (missle_t*)malloc(sizeof(missle_t));
	missle -> x = 20 * _cos + ship -> x;
	missle -> y = -20 * _sin + ship -> y;
	missle -> speed_x = MISSLE_SPEED * _cos;
	missle -> speed_y = MISSLE_SPEED * _sin * (-1);
	missle -> life = MISSLE_LIFE;
	ship -> shoot = 0;
	missle -> next = list;
	return missle;
}

missle_t* update_missles(missle_t* list) {
	missle_t* ret = list;
	missle_t* missle = list;
	missle_t* prev = NULL;
	while (missle != NULL)
	{
		missle -> x += missle -> speed_x;
		missle -> y += missle -> speed_y;
		missle -> life -= 1;
		if (missle -> life == 0)
		{
			if (prev == NULL)
			{
				ret = missle -> next;
				free(missle);
				missle = ret;
			}
			else
			{
				prev -> next = missle -> next;
				free(missle);
				missle = prev -> next;
			}
			continue;
		}
		prev = missle;
		missle = missle -> next;
	}
	return ret;
}

void detect_collissions(world_t* world)
{
	return;
}

void update_world(world_t* world)
{
	update_ship(world -> ship);
	world -> missles = add_missle(world -> missles, world -> ship);
	world -> missles = update_missles(world -> missles);
	world -> asteroids = update_asts(world -> asteroids);
	detect_collissions(world);
}

int main (int argc, char** argv)
{
	SDL_Surface *screen = init_video();
	world_t* world = init_world();
	
	while(1)
	{		
		process_input(world -> ship);
		update_world(world);
		draw(screen, world); /*render*/
		SDL_Delay(16);
	}
}
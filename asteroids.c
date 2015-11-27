#include <SDL/SDL.h>
#include <stdio.h>
#include <math.h>

typedef struct {
	int x;
	int y;
} pixel_t;

typedef struct {
	pixel_t* place;
	double angle;
	int engine;
	int rotating_right;
	int rotating_left;
	int shoot;
	double speed_x;
	double speed_y;
} ship_t;

void cleanup()
{
	printf("Quiting SDL.\n");
	SDL_Quit();
    printf("Quiting....\n");
	exit(0);
}

void putpixel(SDL_Surface *surface, pixel_t* pixel, uint32_t color)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    uint8_t *p = (uint8_t *) surface -> pixels + pixel -> y * surface -> pitch + pixel -> x * bpp;	
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

void line(SDL_Surface* surface, pixel_t* const from, pixel_t* const to, uint32_t color)
{
	double dy = (to -> y) - (from -> y);
	double dx = (to -> x) - (from -> x);
	if (dx >= dy)
	{
		int x;
		double k = dy / dx;
		double b = from -> y + 0.5 - k * (from -> x);
		for (x = from -> x; x <= to -> x; x++)
		{
			int y = (int) floor( k * x + b );
			putpixel(surface, pixel_create(x, y), color);
		}
	}
	else
	{
		int y;
		double k = dx / dy;
		double b = from -> x + 0.5 - k * (from -> y);
		for (y = from -> y; y <= to -> y; y++)
		{
			int x = (int) floor( k * y + b );
			putpixel(surface, pixel_create(x, y), color);
		}
	}
}

void ship_draw(SDL_Surface* screen, ship_t* ship)
{
	pixel_t* center = ship -> place;
	int x = center -> x;
	int y = center -> y;
	uint32_t color = SDL_MapRGB(screen->format, 0xff, 0xff, 0xff);
	line(screen, pixel_create(x - 9, y - 5), pixel_create(x + 9, y + 5), color);
	line(screen, pixel_create(x - 5, y + 5), pixel_create(x + 5, y - 5), color);
}

void draw(SDL_Surface* screen, ship_t* ship)
{
	
	if ( SDL_MUSTLOCK(screen) ) {
		if ( SDL_LockSurface(screen) < 0 ) {
			fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
		}
	}
	
	SDL_FillRect(screen, NULL, 0x000000);
	
	ship_draw(screen, ship);
	
	if ( SDL_MUSTLOCK(screen) ) {
		SDL_UnlockSurface(screen);
	}
	
	SDL_UpdateRect(screen, 0, 0, 0, 0);
}

int main (int argc, char** argv)
{
	SDL_Surface *screen;
	ship_t* ship;
	
	
    printf("Initializing SDL.\n");
	/* Initialize defaults, Video and Audio */
    if((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)==-1)) { 
        fprintf(stderr, "Could not initialize SDL: %s.\n",
						SDL_GetError());
        exit(-1);
    }
    printf("SDL initialized.\n");
	
	atexit(cleanup); /*cleanup on exit*/
	
	screen = SDL_SetVideoMode(640, 480, 8, SDL_SWSURFACE);
    if ( screen == NULL ) {
        fprintf(stderr, "Couldn't set 640x480x8 video mode: %s\n",
                        SDL_GetError());
        exit(1);
    }
	SDL_WM_SetCaption("asteroids", NULL);
	
	ship  = (ship_t*) malloc(sizeof(ship_t));
	ship -> place = pixel_create(70, 70);
	ship -> speed_x = 0;
	ship -> speed_y = 0;
	
	
	while(1)
	{
		SDL_Event event;
		
		draw(screen, ship);
		
		if (ship -> engine)
		{
			double accel = 3;
			ship -> speed_x += accel * cos(ship -> angle);
			ship -> speed_y += accel * sin(ship -> angle);
		}
		else
		{
			double accel = 0.999;
			ship -> speed_x *= accel;
			ship -> speed_y *= accel;
		}
		
		ship -> place -> x += (int) floor(ship -> speed_x + 0.5);
		ship -> place -> y += (int) floor(ship -> speed_y + 0.5);
		
		{
			Uint8* keystate = SDL_GetKeyState(NULL);
			if(keystate[SDLK_LEFT])
			{
			}
			if(keystate[SDLK_RIGHT])
			{
			}
			if(keystate[SDLK_UP])
			{
			}
			
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
								puts("<");
								ship -> speed_x --;
							break;
							
							case SDLK_RIGHT:
								puts(">");
								ship -> speed_x ++;
							break;
							
							case SDLK_UP:
								puts("^");
								ship -> speed_y --;
							break;
							
							case SDLK_DOWN:
								puts("V");
								ship -> speed_y ++;
							break;
							
							default:
							break;
						}				
					break;
					
					case SDL_KEYUP:
						switch (event.key.keysym.sym)
						{
							
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
		
		SDL_Delay(16);
	}
}

int newmain(int argc, char** argv)
{
	int done = 0;
	double timeStepMs;
	
	init();
	timeStepMs = 1000.f / 60;
	init_world();
	
	while (!done)
	{
		timeLastMs = timeCurrentMs;
		timeCurrentMs = SDL_GetTicks();
		timeDeltaMs = timeCurrentMs - timeLastMs;
		timeAccMs += timeDeltaMs;
		
		while (timeAccMs >= timeStepMs)
		{
			processInput();
			updateWorld();
			timeAccMs -= timeStepMs;
		}
		render();
	}
}
#include "asteroids.h"
#include "render.c"
#include "init.c"

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
	double speed = sqrt( sqr(ship -> speed_x) + sqr(ship -> speed_y) );
	if (ship -> rotating_left) 
	{ 	
		ship -> angle += SHIP_ROT_SPEED;
	}
	if (ship -> rotating_right)
	{
		ship -> angle -= SHIP_ROT_SPEED;
	}
	
	if (ship -> engine)
	{
		if ( speed < SHIP_MAX_SPEED ) 
		{
			ship -> speed_x += SHIP_ENGINE_ACCEL * cos(ship -> angle);
			ship -> speed_y -= SHIP_ENGINE_ACCEL * sin(ship -> angle);
		}
	}
	
	ship -> speed_x *= SHIP_SLOWDOWN;
	ship -> speed_y *= SHIP_SLOWDOWN;
	speed = sqrt( sqr(ship -> speed_x) + sqr(ship -> speed_y) );
	if ( (speed < SHIP_MIN_SPEED) & !(ship -> engine) ) 
	{
		ship -> speed_x = 0;
		ship -> speed_y = 0;
	}			
		
	ship -> x += ship -> speed_x;
	ship -> y += ship -> speed_y;
	if (ship -> x > WIDTH) ship -> x -= WIDTH;
	if (ship -> x < 0) ship -> x += WIDTH;
	if (ship -> y > HEIGHT) ship -> y -= HEIGHT;
	if (ship -> y < 0) ship -> y += HEIGHT;
}

asteroid_t* update_asts( asteroid_t* list ) {
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

int in_range(double x, double a, double b)
{
	if (a > b) 
	{
		double r = a;
		a = b;
		b = r;
	}
	if (x < a) return 0;
	if (x > b) return 0;
	return 1;
}

void detect_collisions(world_t* world) 
{
	asteroid_t* ast = world -> asteroids;
	ship_t* ship = world -> ship;
	double k = cos(ship -> angle) / sin( ship -> angle );
	double ship_front_y = -20 * sin( ship -> angle ) + ship -> y;
	while (ast != NULL)
	{
		double dy = k * ( k * ast -> y + ast -> x - ship -> x ) + ship -> y;
		dy /= 1 + sqr(k);
		if ( in_range(dy, ship_front_y, ship -> y) )
		{/*
			printf("collided \n");
			free(ship);
			world -> ship = init_ship();
			*/
		}
		ast = ast -> next;
	}
}

void update_world(world_t* world)
{
	update_ship(world -> ship);
	world -> missles = add_missle(world -> missles, world -> ship);
	world -> missles = update_missles(world -> missles);
	world -> asteroids = update_asts(world -> asteroids);
	detect_collisions(world);
}

int main (int argc, char** argv)
{
	SDL_Surface* screen = init_video();
	world_t* world = init_world();
	
	while(1)
	{		
		process_input(world -> ship);
		update_world(world);
		draw(screen, world); /*render*/
		SDL_Delay(16);
	}
}
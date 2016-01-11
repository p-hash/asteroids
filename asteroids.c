#include "asteroids.h"
#include "init.c"
#include "render.c"

void process_input(ship_t* ship)
{
	SDL_Event event;	
	while(SDL_PollEvent(&event))
	{  
		switch(event.type)
		{  				
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						ship -> lifes = 0;
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
				ship -> lifes = 0;
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
			ship -> speed_x -= SHIP_ENGINE_ACCEL * sin(ship -> angle);
			ship -> speed_y -= SHIP_ENGINE_ACCEL * cos(ship -> angle);
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
	asteroid_t* prev = NULL;
	while (ast != NULL)
	{
		double speed;
		if (ast -> size <= 0)
		{
			if (prev == NULL)
			{
				ret = ast -> next;
				free(ast);
				ast = ret;
			}
			else
			{
				prev -> next = ast -> next;
				free(ast);
				ast = prev -> next;
			}
			continue;
		}
		speed = sqrt(sqr(ast -> speed_x) + sqr(ast -> speed_y));
		if (speed > ASTEROID_MIN_SPEED)
		{
			ast -> speed_x *= ASTEROID_SLOWDOWN;
			ast -> speed_x *= ASTEROID_SLOWDOWN;
		}
		ast -> x += ast -> speed_x;
		ast -> y += ast -> speed_y;
		if (ast -> x > WIDTH) ast -> x -= WIDTH;
		if (ast -> x < 0) ast -> x += WIDTH;
		if (ast -> y > HEIGHT) ast -> y -= HEIGHT;
		if (ast -> y < 0) ast -> y += HEIGHT;
		prev = ast;
		ast = ast -> next;
	}
	if(ret == NULL)
		return init_asteroids();
	return ret;
}

missle_t* add_missle(missle_t* list, ship_t* ship) 
{
	missle_t* missle = NULL;
	double _cos = -cos(ship -> angle);
	double _sin = -sin(ship -> angle);
	if (!(ship -> shoot)) { return list; }
	missle = (missle_t*)malloc(sizeof(missle_t));
	missle -> x = 20 * _sin + ship -> x;
	missle -> y = 20 * _cos + ship -> y;
	missle -> speed_x = MISSLE_SPEED * _sin + ship -> speed_x;
	missle -> speed_y = MISSLE_SPEED * _cos + ship -> speed_y;
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
		if (missle -> x > WIDTH) missle -> x -= WIDTH;
		if (missle -> x < 0) missle -> x += WIDTH;
		if (missle -> y > HEIGHT) missle -> y -= HEIGHT;
		if (missle -> y < 0) missle -> y += HEIGHT;
		missle -> life -= 1;
		if (missle -> life <= 0)
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

double project_pixel(pixel_t* p, pixel_t a)
{
	double x2, y2, xy;
	if (a.x == 0) return (double)(p -> y);
	if (a.y == 0) return (double)(p -> x);
	x2 = a.x * a.x; 
	y2 = a.y * a.y; 
	xy = a.x * a.y; 
	return ( x2 * p -> x + xy * p -> y ) / ( x2 + y2 ); 
}

projection_t project(shape_t* shape, pixel_t axis)
{
	projection_t rslt;
	size_t i;
	double value;
	rslt.max = project_pixel(shape -> nodes, axis);
	rslt.min = rslt.max;
	for (i = 1; i < shape -> count; i++)
	{
		value = project_pixel(shape -> nodes + i, axis);
		if (value > rslt.max) rslt.max = value;
		if (value < rslt.min) rslt.min = value;
	}
	return rslt;
}

int is_not_overlap(shape_t* s1, shape_t* s2, pixel_t axis)
{
	projection_t pr1 = project(s1, axis);
	projection_t pr2 = project(s2, axis);
	int rslt = (pr1.min > pr2.max) || (pr2.min > pr1.max);
	return rslt;
}

int is_collided(shape_t* s1, shape_t* s2)
{
	size_t i;
	pixel_t* nodes, * p1, * p2;
	pixel_t axis;
	for (i = 0; i < s1 -> count; i++) 
	{
		nodes = s1 -> nodes;
		p1 = nodes + i;
		p2 = i + 1 == s1 -> count ? nodes : nodes + i + 1;
		axis.x = p1 -> y - p2 -> y;
		axis.y = p2 -> x - p1 -> x;
		if (is_not_overlap(s1, s2, axis))
			return 0;
	}
	return 1;
}

shape_t* get_msl_shape(missle_t* msl)
{
	shape_t* rslt = (shape_t*)malloc(sizeof(shape_t));
	pixel_t* nodes = (pixel_t*)malloc(2 * sizeof(pixel_t));
	nodes -> x = (int) round(msl -> x);
	nodes -> y = (int) round(msl -> y);
	nodes++;
	nodes -> x = (int) round(msl -> x - msl -> speed_x);
	nodes -> y = (int) round(msl -> y - msl -> speed_y);
	nodes--;
	rslt -> count = 2;
	rslt -> nodes = nodes;
	return rslt;
}

int is_collided_with_msl(missle_t* msl, shape_t* shape)
{
	shape_t* msl_shape = get_msl_shape(msl);
	int rslt = is_collided(shape, msl_shape);
	free_shape(msl_shape);
	return rslt;
}

void detect_collisions(world_t* world) 
{
	asteroid_t* ast = world -> asteroids;
	ship_t* ship = world -> ship;
	shape_t* ship_shape = rotate_shape(SHIP_COLLISION_SHAPE, ship -> x, ship -> y, ship -> angle, 1);
	shape_t* ast_shape;
	while (ast != NULL)
	{
		missle_t* msl = world -> missles;
		missle_t* dtryr = NULL;
		ast_shape = rotate_shape(ASTEROID_COLLISION_SHAPE, ast -> x, ast -> y, 0, ast -> size * 2); 
		if (is_collided(ship_shape, ast_shape))
		{
			free(ship);
			world -> ship = init_ship(world -> ship -> lifes - 1);
			free_shape(ship_shape); 
			free_shape(ast_shape);
			return;
		}
		while (msl != NULL)
		{
			if ((msl -> life != 0) & (is_collided_with_msl(msl, ast_shape)))
			{
				msl -> life = 0;
				dtryr = msl;
			}
			msl = msl -> next;
		}
		if (dtryr != NULL)
		{
			world -> score += 10 * (4 - ast -> size);
			ast -> size--;
			if (ast -> size > 0) 
			{
				asteroid_t* small_ast = (asteroid_t*) malloc(sizeof(asteroid_t));
				small_ast -> size = ast -> size;
				small_ast -> x = ast -> x;
				small_ast -> y = ast -> y;
				small_ast -> speed_x = rand() % 5;
				ast -> speed_x = 2 * ast -> speed_x + 0.01 * dtryr -> speed_x - small_ast -> speed_x;
				if (ast -> speed_x > 5) ast -> speed_x = 5;
				small_ast -> speed_y = rand() % 5;
				ast -> speed_y = 2 * ast -> speed_y + 0.01 * dtryr -> speed_y - small_ast -> speed_y;
				if (ast -> speed_y > 5) ast -> speed_y = 5;
				small_ast -> next = ast -> next;
				ast -> next = small_ast;
				ast = small_ast;
			}
		}
		ast = ast -> next;
		free_shape(ast_shape);
	} 
	free_shape(ship_shape);
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
	srand(time(NULL));
	
	while(world -> ship -> lifes > 0) /*TODO: exit flag */
	{
		process_input(world -> ship);
		update_world(world);
		draw(screen, world); 
		SDL_Delay(16); /*TODO: dynamic delay and fps counter */
	}
		
	display_score(screen, world -> score);
	free_world(world);
	SDL_Delay(2 * 1000); 
	return 0;
}
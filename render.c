#include "asteroids.h"

void putpixel(SDL_Surface *screen, pixel_t pixel, uint32_t color)
{
    int bpp = screen -> format -> BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    uint8_t *p = (uint8_t *) screen -> pixels + pixel.y * screen -> pitch + pixel.x * bpp;	
	if ( (pixel.y < 0) || (pixel.y > HEIGHT) )
		return;
	if ( (pixel.x < 0) || (pixel.x > WIDTH) )
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

pixel_t pixel_create(int const x, int const y)
{
	pixel_t pixel;
	pixel.x = x;
	pixel.y = y;
	return pixel;
}

static void swap(int* a, int* b) 
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

pixel_t pixel_create_from(pixel_t o, double dx, double dy, double angle)
{
	int x = (int) round(dx * cos(angle) + dy * sin(angle));
	int y = (int) round( -1 * dx * sin(angle) + dy * cos(angle));
	x += o.x;
	y += o.y;
	return pixel_create(x, y);
}

shape_t* rotate_shape(shape_t* shape, double x, double y, double angle, double scale)
{
	size_t i;
	shape_t* rslt = (shape_t*) malloc(sizeof(shape_t));
	pixel_t* rotated = (pixel_t*) malloc(shape -> count * sizeof(pixel_t));
	pixel_t center = pixel_create((int)round(x), (int)round(y));
	for (i = 0; i < shape -> count; i++)
	{
		pixel_t* node = shape -> nodes + i;
		pixel_t pxl = pixel_create_from(center, node -> x * scale, node -> y * scale, angle);
		rotated[i] = pxl;
	}
	rslt -> count = shape -> count;
	rslt -> color = shape -> color;
	rslt -> nodes = rotated;
	return rslt;
}

void draw_shape(SDL_Surface* screen, shape_t* shape)
{
	size_t i;
	pixel_t* nodes = shape -> nodes;
	pixel_t* p1, * p2;
	for (i = 0; i < shape -> count; i++)
	{
		p1 = nodes + i;
		p2 = i + 1 == shape -> count ? nodes : nodes + i + 1;
		line(screen, p1, p2, shape -> color);
	}
}

void draw_ship(SDL_Surface* screen, ship_t* ship)
{
	shape_t* shape = rotate_shape(SHIP_DRAWING_SHAPE, ship -> x, ship -> y, ship -> angle, 1);
	draw_shape(screen, shape);
	free_shape(shape);
	if (ship -> engine)
	{
		shape = rotate_shape(ENGINE_DRAWING_SHAPE, ship -> x, ship -> y, ship -> angle, 1);
		draw_shape(screen, shape);
		free_shape(shape);
	}
}

void draw_asts(SDL_Surface* screen, asteroid_t* ast)
{
	shape_t* shape;
	while (ast != NULL)
	{
		shape = rotate_shape(ASTEROID_DRAWING_SHAPE, ast -> x, ast -> y, 0, ast -> size * 2);
		draw_shape(screen, shape);
		free_shape(shape);
		ast = ast -> next;
	}
}

void draw_missles(SDL_Surface* screen, missle_t* list)
{
	uint32_t color = SDL_MapRGB(screen -> format, 0xff, 0xff, 0x00);
	while (list != NULL)
	{
		putpixel(screen, pixel_create((int)list -> x, (int)list -> y), color);
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

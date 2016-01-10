#include <SDL/SDL.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#ifndef ASTEROIDS_HEADERS
#define ASTEROIDS_HEADERS

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define EPS 0.000001
#define WIDTH 640
#define HEIGHT 480

#define SHIP_MAX_SPEED 20
#define SHIP_MIN_SPEED 0.01
#define SHIP_ROT_SPEED 0.08
#define SHIP_ENGINE_ACCEL 0.075
#define SHIP_SLOWDOWN 0.995

#define MISSLE_SPEED 10
#define MISSLE_LIFE 40

#define ASTEROID_MAX_SPEED 15
#define ASTEROID_SLOWDOWN 0.997
#define ASTEROID_MIN_SPEED 5

typedef struct {
	int x;
	int y;
} pixel_t;

typedef struct {
	double min;
	double max;
} projection_t;

typedef struct {
	pixel_t* nodes;
	size_t count;
	uint32_t color;
} shape_t;

shape_t* SHIP_DRAWING_SHAPE;
shape_t* ENGINE_DRAWING_SHAPE;
shape_t* SHIP_COLLISION_SHAPE;
shape_t* ASTEROID_DRAWING_SHAPE;
shape_t* ASTEROID_COLLISION_SHAPE;

typedef struct {
	double k;
	double dx;
	double dy;
} axis_t;

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

#endif
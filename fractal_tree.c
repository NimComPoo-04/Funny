#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

// #define FRAMES  ;; Defined in the makefile
// #define WIDTH
// #define HEIGHT

#define PI 3.1415

const uint32_t Green = 0x00ff00;		// Green
const uint32_t Blue  = 0xffff00;		// Blue

uint32_t *screen_buffer = NULL;

// Puts a pixel on single magenta pixel on screen
void putpixel(int x, int y)
{
	int fy = (y + WIDTH / 2);
	int fx = (HEIGHT - 1) - (x + HEIGHT / 2.6);

	if(fy < 0 || fy >= WIDTH) return;
	if(fx < 0 || fx >= HEIGHT) return;

	screen_buffer[fx * WIDTH + fy] = Green;
}

// Draws a scuffed line from point 1 to 2
// TODO: don't use floats 'k its bad for thy health, just use bresenham
void drawline(int x1, int y1, int x2, int y2)
{
	if(abs(x2 - x1) >= abs(y2 - y1))
	{
		if(x1 > x2)
		{
			int f = x1;
			x1 = x2;
			x2 = f;

			f = y1;
			y1 = y2;
			y2 = f;
		}

		float x = x1;
		float y = y1;

		float m = 1.0 * (y2 - y1)/(x2 - x1);
		while(x <= x2)
		{
			putpixel((int)x, (int)y);
			y += m;
			x += 1;
		}
	}
	else
	{
		if(y1 > y2)
		{
			int f = y1;
			y1 = y2;
			y2 = f;

			f = x1;
			x1 = x2;
			x2 = f;
		}

		float x = x1;
		float y = y1;
		float m = 1.0 * (x2 - x1) / (y2 - y1);

		while(y <= y2)
		{
			putpixel((int)x, (int)y);
			x += m;
			y += 1;
		}
	}
}

// Fractal stuff
const float Angle = PI * 2 / 3;
const uint32_t Iterations = 10;

typedef struct
{
	int ox; int oy;
	int x; int y;
	float a; int r;
} node_t;

node_t *last_generation = NULL;
size_t  generation_size = 0;

// the name is scuffed lol
void generate_generation()
{
	for(size_t j = 0; j < generation_size; j++)
	{
		size_t i = generation_size - j - 1;
		node_t *n = last_generation + i;

		float ai = n->a + Angle;
		float an = n->a ;
		float ad = n->a - Angle;

		last_generation[3 * i + 2] = (node_t){
			n->x, n->y,
			n->x + cosf(ad) * n->r,
			n->y + sinf(ad) * n->r,
			ad, n->r / 1.8
		};

		last_generation[3 * i + 1] = (node_t){
			n->x, n->y,
			n->x + cosf(an) * n->r,
			n->y + sinf(an) * n->r,
			an, n->r / 1.8
		};

		last_generation[3 * i] = (node_t){
			n->x, n->y,
			n->x + cosf(ai) * n->r,
			n->y + sinf(ai) * n->r,
			ai, n->r / 1.8
		};
	}

	generation_size *= 3;
}

// drawing the generations
void draw_generation()
{
	for(size_t i = 0; i < generation_size; i++)
	{
		drawline(last_generation[i].ox, last_generation[i].oy, last_generation[i].x, last_generation[i].y);
	//	drawline(-last_generation[i].ox, last_generation[i].oy, -last_generation[i].x, last_generation[i].y);
	}
}

int main(void)
{
	screen_buffer = calloc(sizeof(uint32_t), WIDTH * HEIGHT);
	last_generation = calloc(sizeof(node_t), (int)pow(3, Iterations));

	last_generation[0].r = 200;
	generation_size = 1;

	int exist = (FRAMES / Iterations);

	for(int i = 0; i < FRAMES; i++)
	{
		if(i % exist == 0)
		{
			generate_generation();
			draw_generation();
		}

		fwrite(screen_buffer, WIDTH * HEIGHT, sizeof(uint32_t), stdout);
	}

	free(screen_buffer);
	free(last_generation);
}

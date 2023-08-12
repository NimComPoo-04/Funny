#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>
#include <stdint.h>

#define N 4

// Technical Details
static uint32_t *screen_buffer = NULL;

static const int Iterations = FRAMES;
static int LeafColor = 0x7aef85;

static float position[2] = {0, 0};

void put_pixel(float x, float y, uint32_t color)
{
	int fx = (x + 5) / 10 * WIDTH;
	int fy = (10 - y) / 10 * HEIGHT;

	if(fx >= WIDTH || fx < 0) return;
	if(fy >= HEIGHT || fy < 0) return;

	screen_buffer[fy * WIDTH + fx] = color;
}

void update_fern()
{

#define X position[0]
#define Y position[1]

	float x = 0;
	float y = 0;

	int rx = rand() % 100;

	if(rx < 10)
	{
		x = 0.0;
		y = 0.16 * Y;
	}
	else if(rx < 86)
	{
		x = 0.85 * X + 0.04 * Y;
		y = -0.04 * X + 0.85 * Y + 1.6;
	}
	else if(rx < 93)
	{
		x = 0.2 * X - 0.26 * Y;
		y = 0.23 * X + 0.22 * Y + 1.6;
	}
	else
	{
		x = -0.15 * X + 0.22 * Y;
		y = 0.26 * X + 0.24 * Y + 0.44;
	}

	X = x;
	Y = y;

#undef X
#undef Y
}

int main(void)
{
#define SIZE sizeof(uint32_t) * WIDTH * HEIGHT
	screen_buffer = malloc(SIZE);
	memset(screen_buffer, 0, SIZE);

	for(int i = 0; i < Iterations; i++)
	{
		for(int i = 0; i < 500; i++)
		{
			put_pixel(position[0], position[1], LeafColor);
			update_fern();
		}
		fwrite(screen_buffer, SIZE, 1, stdout);
	}

	return 0;
}

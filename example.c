/*
A simple example of using the gfx library.
CSE 20211
9/7/2011
by Prof. Thain
*/

#include <stdio.h>
#include <stdlib.h>
#include "gfx.h"
#include "geometry.h" 
#include "algos.h" 
#include <unistd.h> 


int main()
{
	int ysize = 980;
	int xsize = 1280;

	//char c;

	// Open a new window for drawing.
	gfx_open(xsize,ysize,"Example Graphics Program");

	// Set the current drawing color to green.
	gfx_color(0,200,100);

	// Draw triangles and animate 
	Queue* q = malloc(sizeof(Queue));
	queue_init(q);
	Queue* q12 = malloc(sizeof(Queue));
	Queue* q13 = malloc(sizeof(Queue));
	Queue* q23 = malloc(sizeof(Queue));
	queue_init(q12);
	queue_init(q13);
	queue_init(q23);
	gfx_color(50,200,0);

	//vec2i_t pt1 = {10, 20}, pt2 = {250, 20};
	Pixel pt1 = {{300, 20}, {255, 0, 0}};
	Pixel pt2 = {{840, 650}, {0, 255, 0}};
	Pixel pt3 = {{100, 800}, {0, 0, 255}};

	gfx_triangle_fill_bres(&pt1, &pt2, &pt3, q12, q13, q23);

	Queue* q45 = malloc(sizeof(Queue));
	Queue* q46 = malloc(sizeof(Queue));
	Queue* q56 = malloc(sizeof(Queue));
	queue_init(q45);
	queue_init(q46);
	queue_init(q56);
	Pixel pt4 = {{800, 80}, {0, 0, 255}};
	Pixel pt5 = {{500, 300}, {0, 255, 0}};
	Pixel pt6 = {{700, 600}, {255, 0, 0}};
	gfx_triangle_fill_bres(&pt4, &pt5, &pt6, q45, q46, q56);

	gfx_flush();
	sleep(30);
	gfx_clear();

	queue_del(q12);
	queue_del(q13);
	queue_del(q23);
	queue_del(q);
#if 0
	int i;
	float deg_step = -10.0;

	for (i = 0; i < 15; i++) {
		gfx_color(50,0,200);
		gfx_triangle_fill_int_test(&tr31, &tr32, &tr33);
		geo_rotateTriangle(&tr31, &tr32, &tr33, deg_step);
		gfx_color(50,200,0);
		gfx_triangle_fill_int_test(&tr21, &tr22, &tr23);
		geo_rotateTriangle(&tr21, &tr22, &tr23, 2*deg_step);
		gfx_color(200,50,0);
		gfx_triangle_fill_int_test(&tr11, &tr12, &tr13);
		geo_rotateTriangle(&tr11, &tr12, &tr13, 3*deg_step);
		gfx_flush();
		sleep(1);
		gfx_clear();
	}
	// queue_del(q);
	// Wait for the user to press a character.
	//char c = gfx_wait();
	// Quit if it is the letter q.
	//if (c=='q') break;
#endif
	return 0;
}

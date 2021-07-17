/*
A simple example of using the gfx library.
CSE 20211
9/7/2011
by Prof. Thain
17/07/2021
by Leontios Mavropalias
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

	Pixel pt1 = {{300, 20}, {255, 0, 0}};
	Pixel pt2 = {{840, 650}, {0, 255, 0}};
	Pixel pt3 = {{100, 800}, {0, 0, 255}};

	algos_triangle_fill_bres(&pt1, &pt2, &pt3, q12, q13, q23);

	Queue* q45 = malloc(sizeof(Queue));
	Queue* q46 = malloc(sizeof(Queue));
	Queue* q56 = malloc(sizeof(Queue));
	queue_init(q45);
	queue_init(q46);
	queue_init(q56);
	Pixel pt4 = {{800, 80}, {0, 0, 255}};
	Pixel pt5 = {{500, 300}, {0, 255, 0}};
	Pixel pt6 = {{700, 600}, {255, 0, 0}};
	algos_triangle_fill_bres(&pt4, &pt5, &pt6, q45, q46, q56);

	gfx_flush();
	sleep(5);
	gfx_clear();

	queue_del(q12);
	queue_del(q13);
	queue_del(q23);
	queue_del(q45);
	queue_del(q56);
	queue_del(q46);
	queue_del(q);
	free(q12);
	free(q13);
	free(q23);
	free(q45);
	free(q56);
	free(q46);
	free(q);
	return 0;
}

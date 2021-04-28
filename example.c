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
#include <unistd.h> 

int main()
{
	int ysize = 320;
	int xsize = 640;

	//char c;

	// Open a new window for drawing.
	gfx_open(xsize,ysize,"Example Graphics Program");

	// Set the current drawing color to green.
	gfx_color(0,200,100);

	// Draw triangles and animate 
	Queue* q = malloc(sizeof(Queue));
	queue_init(q);
	gfx_color(50,200,0);
	Point tr11 = {190, 200, 1}, tr12 = {200, 160, 1}, tr13 = {210,200, 1};
	Point tr21 = {180, 200, 1}, tr22 = {200, 140, 1}, tr23 = {220, 200, 1};
	Point tr31 = {165, 200, 1}, tr32 = {200, 100, 1}, tr33 = {235, 200, 1};

	int i;
	float deg_step = -10.0;

	for (i = 0; i < 15; i++) {
		gfx_color(50,0,200);
		gfx_triangle_fill_int_test(tr31.x, tr31.y, tr32.x, tr32.y, tr33.x, tr33.y);
		geo_rotateTriangle(&tr31, &tr32, &tr33, deg_step);
		gfx_color(50,200,0);
		gfx_triangle_fill_int_test(tr21.x, tr21.y, tr22.x, tr22.y, tr23.x, tr23.y);
		geo_rotateTriangle(&tr21, &tr22, &tr23, 2*deg_step);
		gfx_color(200,50,0);
		gfx_triangle_fill_int_test(tr11.x, tr11.y, tr12.x, tr12.y, tr13.x, tr13.y);
		geo_rotateTriangle(&tr11, &tr12, &tr13, 3*deg_step);
		gfx_flush();
		sleep(1);
		gfx_clear();
	}
	// Wait for the user to press a character.
	//c = gfx_wait();
	// Quit if it is the letter q.
	//if (c=='q') break;
	queue_del(q);

	return 0;
}

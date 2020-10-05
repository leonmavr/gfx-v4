/*
A simple example of using the gfx library.
CSE 20211
9/7/2011
by Prof. Thain
*/

#include <stdio.h>
#include <stdlib.h>
#include "gfx.h"
#include "rotations.h" 
#include <unistd.h> 

int main()
{
	int ysize = 320;
	int xsize = 640;

	char c;

	// Open a new window for drawing.
	gfx_open(xsize,ysize,"Example Graphics Program");

	// Set the current drawing color to green.
	gfx_color(0,200,100);

	// Draw line
	Queue* q = malloc(sizeof(Queue));
	queue_init(q);
	gfx_color(50,200,0);
	Point pt1 = {200, 200, 1}, pt2 = {250, 240,1}, pt3 = {190, 260, 1};
	//gfx_triangle_fill(pt1.x, pt1.y, pt2.x, pt2.y, pt3.x, pt3.y);
	gfx_color(200,50,0);
	int i;
	for (i = 0; i < 40; i++) {
		if (i/2 == 0){
			gfx_color(200,50,0); printf("here\n");
		}
		else{
			gfx_color(50,100,0); printf("there\n");
		}
		gfx_triangle_fill(pt1.x, pt1.y, pt2.x, pt2.y, pt3.x, pt3.y);
		rotateTriangle(&pt1, &pt2, &pt3, 5);
		gfx_flush();
		sleep(1.0);
		gfx_clear();
	}
	queue_del(q);

	while(1) {
		// Wait for the user to press a character.
		c = gfx_wait();

		// Quit if it is the letter q.
		if(c=='q') break;
	}

	return 0;
}

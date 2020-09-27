/*
A simple example of using the gfx library.
CSE 20211
9/7/2011
by Prof. Thain
*/

#include <stdio.h>
#include "gfx.h"

int main()
{
	int ysize = 320;
	int xsize = 640;

	char c;

	// Open a new window for drawing.
	gfx_open(xsize,ysize,"Example Graphics Program");

	// Set the current drawing color to green.
	gfx_color(0,200,100);

	// Draw a triangle on the screen.
	//gfx_line(100,100,200,100);
	//gfx_line(200,100,150,150);
	//gfx_line(150,150,100,100);
	
	// Draw line
	gfx_color(50,200,0);
	gfx_line_bres(100,100, 80, 160);
	gfx_line_bres(100,100, 20, 120);
	gfx_line_bres(100,100, 20, 80);
	gfx_line_bres(100,100, 80, 20);
	gfx_line_bres(100,100, 140, 20);
	gfx_line_bres(100,100, 160, 80);
	gfx_color(200, 50,0);
	gfx_line(97,97, 140, 20);
	gfx_line(95,97, 160, 80);
	//gfx_line(8, 18, 320, 50);

	while(1) {
		// Wait for the user to press a character.
		c = gfx_wait();

		// Quit if it is the letter q.
		if(c=='q') break;
	}

	return 0;
}

/*
   A simple graphics library for CSE 20211 by Douglas Thain

   This work is licensed under a Creative Commons Attribution 4.0 International License.  https://creativecommons.org/licenses/by/4.0/

   For complete documentation, see:
http://www.nd.edu/~dthain/courses/cse20211/fall2013/gfx
Version 3, 11/07/2012 - Now much faster at changing colors rapidly.
Version 2, 9/23/2011 - Fixes a bug that could result in jerky animation.
*/

#include <X11/Xlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "gfx.h"
#include "queue.h" 

/*
   gfx_open creates several X11 objects, and stores them in globals
   for use by the other functions in the library.
   */

static Display *gfx_display=0;
static Window  gfx_window;
static GC      gfx_gc;
static Colormap gfx_colormap;
static int      gfx_fast_color_mode = 0;

/* These values are saved by gfx_wait then retrieved later by gfx_xpos and gfx_ypos. */

static int saved_xpos = 0;
static int saved_ypos = 0;

/* Open a new graphics window. */

void gfx_open( int width, int height, const char *title )
{
	gfx_display = XOpenDisplay(0);
	if(!gfx_display) {
		fprintf(stderr,"gfx_open: unable to open the graphics window.\n");
		exit(1);
	}

	Visual *visual = DefaultVisual(gfx_display,0);
	if(visual && visual->class==TrueColor) {
		gfx_fast_color_mode = 1;
	} else {
		gfx_fast_color_mode = 0;
	}

	int blackColor = BlackPixel(gfx_display, DefaultScreen(gfx_display));
	int whiteColor = WhitePixel(gfx_display, DefaultScreen(gfx_display));

	gfx_window = XCreateSimpleWindow(gfx_display, DefaultRootWindow(gfx_display), 0, 0, width, height, 0, blackColor, blackColor);

	XSetWindowAttributes attr;
	attr.backing_store = Always;

	XChangeWindowAttributes(gfx_display,gfx_window,CWBackingStore,&attr);

	XStoreName(gfx_display,gfx_window,title);

	XSelectInput(gfx_display, gfx_window, StructureNotifyMask|KeyPressMask|ButtonPressMask);

	XMapWindow(gfx_display,gfx_window);

	gfx_gc = XCreateGC(gfx_display, gfx_window, 0, 0);

	gfx_colormap = DefaultColormap(gfx_display,0);

	XSetForeground(gfx_display, gfx_gc, whiteColor);

	// Wait for the MapNotify event

	for(;;) {
		XEvent e;
		XNextEvent(gfx_display, &e);
		if (e.type == MapNotify)
			break;
	}
}

/* Draw a single point at (x,y) */

void gfx_point( int x, int y )
{
	XDrawPoint(gfx_display,gfx_window,gfx_gc,x,y);
}


static unsigned int find_octant(int x1, int y1, int x2, int y2) {
	if (x1 == x2)
		return 8;
	float m = (float)(y2 - y1)/(x2 - x1);
	if ((x1 <= x2) && (0 <= m) && (m <= 1))
		return 0;
	else if ((y1 <= y2) && (m > 1))
		return 1;
	else if ((y1 <= y2) && (m < -1))
		return 2;
	else if ((x2 <= x1) && (0 >= m) && (m >= -1))
		return 3;
	else if ((x2 <= x1) && (0 < m) && (m <= 1))
		return 4;
	else if ((y2 <= y1) && (m > 1))
		return 5;
	else if ((y2 <= y1) && (m < -1))
		return 6;
	else if ((x1 <= x2) && (-1 <= m) && (m <= 0))
		return 7;
}


/* Draw a line from (x1,y1) to (x2,y2) using Bresenham's */
void gfx_line_bres(int x1, int y1, int x2, int y2, Queue* q)
{
	int dx = x2 - x1;
	int dy = y2 - y1;
	float m = (float)dy/dx;
	int err = 0;
	int y = y1, x = x1;
	Point pt;
	unsigned int oct = find_octant(x1, y1, x2, y2);
	switch(oct) {
		/* octant 1 */
		case 0: 
			y = y1;
			for (x = x1; x < x2; x++) {
				gfx_point(x, y);
				pt.x = x;
				pt.y = y;
				pt.valid = 1;
				append(q, pt);
				err += dy;
				if (2*err >= dx){
					err -= dx;
					y++;
				}
			}
			break;
			/* octant 2 */
		case 1:
			x = x1;
			for (y = y1; y < y2; y++) {
				gfx_point(x, y);
				pt.x = x;
				pt.y = y;
				pt.valid = 1;
				append(q, pt);
				err += dx;
				if (2*err >= dy){
					err -= dy;
					x++;
				}
			}
			break;
			/* octant 3 */
		case 2:
			x = x1;
			for (y = y1; y < y2; y++) {
				gfx_point(x, y);
				pt.x = x;
				pt.y = y;
				pt.valid = 1;
				append(q, pt);
				err -= dx;
				if (2*err >= dy){
					err -= dy;
					x--;
				}
			}
			break;
			/* octant 4 */
		case 3:
			y = y1;
			for (x = x1; x > x2; x--) {
				gfx_point(x, y);
				pt.x = x;
				pt.y = y;
				pt.valid = 1;
				append(q, pt);
				err += dy;
				if (2*err >= -dx){
					err += dx;
					y++;
				}
			}
			break;
			/* octant 5 */
		case 4:
			y = y1;
			for (x = x1; x > x2; x--) {
				gfx_point(x, y);
				pt.x = x;
				pt.y = y;
				pt.valid = 1;
				append(q, pt);
				err -= dy;
				if (2*err >= -dx){
					err += dx;
					y--;
				}
			}
			break;
			/* octant 6 */
		case 5:
			x = x1;
			for (y = y1; y > y2; y--) {
				gfx_point(x, y);
				pt.x = x;
				pt.y = y;
				pt.valid = 1;
				append(q, pt);
				err -= dx;
				if (2*err >= -dy){
					err -= dy;
					x--;
				}
			}
			break;
			/* octant 6 */
		case 6:
			x = x1;
			for (y = y1; y > y2; y--) {
				gfx_point(x, y);
				pt.x = x;
				pt.y = y;
				pt.valid = 1;
				append(q, pt);
				err += dx;
				if (2*err >= -dy){
					err += dy;
					x++;
				}
			}
			break;
			/* octant 6 */
		case 7:
			y = y1;
			for (x = x1; x < x2; x++) {
				gfx_point(x, y);
				pt.x = x;
				pt.y = y;
				pt.valid = 1;
				append(q, pt);
				err -= dy;
				if (2*err >= dx){
					err -= dx;
					y--;
				}
			}
			break;
			/* octant 7 */
		case 8:
			if (y1 < y2){
				for (y = y1; y < y2; y++) {
					gfx_point(x, y);	
					pt.x = x;
					pt.y = y;
					pt.valid = 1;
					append(q, pt);
				}
			} else {
				for (y = y2; y < y1; y++) {
					gfx_point(x, y);	
					pt.x = x;
					pt.y = y;
					pt.valid = 1;
					append(q, pt);
				}
			}
			break;
	}
}


/**
 * @brief Given a Queue of points, if it encounters consecutive points
 * with the same y, keeps only the last one. This is required for triangle
 * rasterisation.
 *
 * @param q pointer to Queue
 *
 * @return a Queue pointer whose y's are all sequentially incremented
 */
static Queue* gfx_removeDuplYs(Queue* q){
	Queue* qFinal = malloc(sizeof(Queue));
	init(qFinal);
	Point ptOld = {.x=0, .y=0, .valid=0};
	Point pt = {.x=0, .y=0, .valid=0};

	do{
		pt = pop(q);
		if((pt.y != ptOld.y) && (pt.valid == 1)) 
			append(qFinal, pt);
		ptOld = pt;
	} while (pt.valid == 1);

	return qFinal;
}


void gfx_triangle_fill(int x1, int  y1, int x2, int y2, int x3,int y3) {
	/*
	 *                           X (x1, y1)
	 *                          /| 
	 *                         / |
	 *                        /  |
	 *                       /  |
	 *             (x2,y2)  X   |
	 *                      \   |
	 *                       \  |
	 *                        \ |
	 *                         \|
	 *                          X (x3, y3)
	 */ 
	/*****************************************
	 * Initialisation 
	 *****************************************/
	assert((y1 <= y2) && (y2 <= y3));
	Queue* q1, *q2, *q3, *qBuff;
	Queue* q1new, *q2new, *q3new;
	q1 = malloc(sizeof(Queue));
	q2 = malloc(sizeof(Queue));
	q3 = malloc(sizeof(Queue));
	qBuff = malloc(sizeof(Queue));
	init(q1);
	init(q2);
	init(q3);
	init(qBuff);

	/*****************************************
	 * Main work (rasterisation)
	 *****************************************/
	gfx_line_bres(x1, y1, x2, y2, q1);
	gfx_line_bres(x2, y2, x3, y3, q2);
	gfx_line_bres(x1, y1, x3, y3, q3);

	q1new = gfx_removeDuplYs(q1);
	q2new = gfx_removeDuplYs(q2);
	q3new = gfx_removeDuplYs(q3);

	Point pt12 = {.x=0,.y=0,.valid=1};
	Point pt23 = {.x=0,.y=0,.valid=1};
	Point pt13 = {.x=0,.y=0,.valid=1};

	int y;
	Point pt = {.x=0, .y=0, .valid=0};
	for (y = y1; (pt13.valid == 1) &&  (y < y3);){
		if (y < y2){
			pt12 = pop(q1new);
			pt13 = pop(q3new);
			gfx_line_bres(pt12.x, pt12.y, pt13.x, pt13.y, qBuff);
			y++;
		} else{
			pt23 = pop(q2new);
			pt13 = pop(q3new);
			gfx_line_bres(pt23.x, pt23.y, pt13.x, pt13.y, qBuff);
			y++;
		}
	}
	
	/*****************************************
	 * Cleanup
	 *****************************************/
	del(q1);
	del(q2);
	del(q3);
	del(q1new);
	del(q2new);
	del(q3new);
	del(qBuff);
	free(q1);
	free(q2);
	free(q3);
	free(q1new);
	free(q2new);
	free(q3new);
	free(qBuff);
}

/* Draw a line from (x1,y1) to (x2,y2) */

void gfx_line( int x1, int y1, int x2, int y2 )
{
	XDrawLine(gfx_display,gfx_window,gfx_gc,x1,y1,x2,y2);
}

/* Change the current drawing color. */

void gfx_color( int r, int g, int b )
{
	XColor color;

	if(gfx_fast_color_mode) {
		/* If this is a truecolor display, we can just pick the color directly. */
		color.pixel = ((b&0xff) | ((g&0xff)<<8) | ((r&0xff)<<16) );
	} else {
		/* Otherwise, we have to allocate it from the colormap of the display. */
		color.pixel = 0;
		color.red = r<<8;
		color.green = g<<8;
		color.blue = b<<8;
		XAllocColor(gfx_display,gfx_colormap,&color);
	}

	XSetForeground(gfx_display, gfx_gc, color.pixel);
}

/* Clear the graphics window to the background color. */

void gfx_clear()
{
	XClearWindow(gfx_display,gfx_window);
}

/* Change the current background color. */

void gfx_clear_color( int r, int g, int b )
{
	XColor color;
	color.pixel = 0;
	color.red = r<<8;
	color.green = g<<8;
	color.blue = b<<8;
	XAllocColor(gfx_display,gfx_colormap,&color);

	XSetWindowAttributes attr;
	attr.background_pixel = color.pixel;
	XChangeWindowAttributes(gfx_display,gfx_window,CWBackPixel,&attr);
}

int gfx_event_waiting()
{
	XEvent event;

	gfx_flush();

	while (1) {
		if(XCheckMaskEvent(gfx_display,-1,&event)) {
			if(event.type==KeyPress) {
				XPutBackEvent(gfx_display,&event);
				return 1;
			} else if (event.type==ButtonPress) {
				XPutBackEvent(gfx_display,&event);
				return 1;
			} else {
				return 0;
			}
		} else {
			return 0;
		}
	}
}

/* Wait for the user to press a key or mouse button. */

char gfx_wait()
{
	XEvent event;

	gfx_flush();

	while(1) {
		XNextEvent(gfx_display,&event);

		if(event.type==KeyPress) {
			saved_xpos = event.xkey.x;
			saved_ypos = event.xkey.y;
			return XLookupKeysym(&event.xkey,0);
		} else if(event.type==ButtonPress) {
			saved_xpos = event.xkey.x;
			saved_ypos = event.xkey.y;
			return event.xbutton.button;
		}
	}
}

/* Return the X and Y coordinates of the last event. */

int gfx_xpos()
{
	return saved_xpos;
}

int gfx_ypos()
{
	return saved_ypos;
}

/* Flush all previous output to the window. */

void gfx_flush()
{
	XFlush(gfx_display);
}


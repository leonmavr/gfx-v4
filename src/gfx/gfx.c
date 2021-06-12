/*
   A simple graphics library for CSE 20211 by Douglas Thain
   Modified in 2020 by Leontios Mavropalias.

   This work is licensed under a Creative Commons Attribution 4.0 International License.  https://creativecommons.org/licenses/by/4.0/

   For complete documentation, see:
http://www.nd.edu/~dthain/courses/cse20211/fall2013/gfx
Version 4, 11/06/2020 - Support drawing triangles and lines
Version 3, 11/07/2012 - Now much faster at changing colors rapidly.
Version 2, 9/23/2011 - Fixes a bug that could result in jerky animation.
*/

#include <X11/Xlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "gfx.h"
#include "queue.h" 


/*
 * Generic swap - original at https://rosettacode.org/wiki/Generic_swap#C
 */
#define SWAP(X,Y)  do{ __typeof__ (X) _T = X; X = Y; Y = _T; }while(0)
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define PERP_DOT(a, b, c, d) (a*d - c*b)

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


static inline void vec3u_add(vec3u_t* result, vec3u_t* vec1, vec3u_t* vec2) {
	result->x = vec1->x + vec2->x;	
	result->y = vec1->y + vec2->y;	
	result->z = vec1->z + vec2->z;	
}


static inline void vec3u_mul_float(vec3u_t* result, vec3u_t* vec1, float fl) {
	result->x = fl * vec1->x;
	result->y = fl * vec1->y;
	result->z = fl * vec1->z;
}


/* Find which octant a line belongs in */
static unsigned int gfx_findOctant(const vec2i_t* pt1, const vec2i_t* pt2) {
	int x1 = pt1->x;
	int y1 = pt1->y;
	int x2 = pt2->x;
	int y2 = pt2->y;

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
	else // if ((x1 <= x2) && (-1 <= m) && (m <= 0))
		return 7;
}


/* Draw a line from (x1,y1) to (x2,y2) using Bresenham's */
void gfx_line_bres(const vec2i_t* pt1, const vec2i_t* pt2, Queue* q)
{
	int x1 = pt1->x;
	int y1 = pt1->y;
	int x2 = pt2->x;
	int y2 = pt2->y;
	
	int dx = x2 - x1;
	int dy = y2 - y1;
	int err = 0;
	int y = y1, x = x1;
	Pixel pt;
	unsigned int oct = gfx_findOctant(pt1, pt2);
	switch(oct) {
		/* octant 1 */
		case 0: 
			y = y1;
			for (x = x1; x < x2; x++) {
				gfx_point(x, y);
				pt.pt.x = x;
				pt.pt.y = y;
				queue_append(q, &pt);
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
				pt.pt.x = x;
				pt.pt.y = y;
				queue_append(q, &pt);
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
				pt.pt.x = x;
				pt.pt.y = y;
				queue_append(q, &pt);
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
				pt.pt.x = x;
				pt.pt.y = y;
				queue_append(q, &pt);
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
				pt.pt.x = x;
				pt.pt.y = y;
				queue_append(q, &pt);
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
				pt.pt.x = x;
				pt.pt.y = y;
				queue_append(q, &pt);
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
				pt.pt.x = x;
				pt.pt.y = y;
				queue_append(q, &pt);
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
				pt.pt.x = x;
				pt.pt.y = y;
				queue_append(q, &pt);
				err -= dy;
				if (2*err >= dx){
					err -= dx;
					y--;
				}
			}
			break;
		/* octant 7 */
		default:
			if (y1 < y2){
				for (y = y1; y < y2; y++) {
					gfx_point(x, y);	
					pt.pt.x = x;
					pt.pt.y = y;
					queue_append(q, &pt);
				}
			} else {
				for (y = y2; y < y1; y++) {
					gfx_point(x, y);	
					pt.pt.x = x;
					pt.pt.y = y;
					queue_append(q, &pt);
				}
			}
			break;
	}
}


void gfx_line_bres_col(Pixel* pt1, Pixel* pt2, Queue* q) {
	gfx_line_bres(&pt1->pt, &pt2->pt, q);
	int len = queue_length(q);
	vec3u_t col1 = pt1->colour;
	vec3u_t col2 = pt2->colour;
	vec3u_t col1_curr = col1;
	vec3u_t col2_curr = col2;
	vec3u_t col_curr;
	Pixel* curr = q->head;

	for (int i = 0; i < len; ++i) {
		//c12.x = (float)(n12 - i12 - 1)/n12*col1->x + (float)(i12 -1)/n12*col2->x;
		vec3u_mul_float(&col1_curr, &col1,  (float)(len-i-1)/len);
		vec3u_mul_float(&col2_curr, &col2,  (float)(i -1)/len);
		vec3u_add(&col_curr, &col1_curr, &col2_curr);
		// draw using the resulting colour
		gfx_color(col_curr.x, col_curr.y, col_curr.z);
		curr->colour.x = col_curr.x;
		curr->colour.y = col_curr.y;
		curr->colour.z = col_curr.z;
		gfx_point(curr->pt.x, curr->pt.y);
		gfx_flush();
		curr = curr->next;
	}
}


void gfx_triangle_fill_bres(Pixel* pt1, Pixel* pt2, Pixel* pt3,
		Queue* q12, Queue* q13, Queue* q23) {
	// Assume y1 <= y2 <= y3 !!!
	assert((pt1->pt.y <= pt2->pt.y) && (pt2->pt.y <= pt3->pt.y));

	gfx_line_bres_col(pt1, pt2, q12);
	gfx_line_bres_col(pt1, pt3, q13);
	gfx_line_bres_col(pt2, pt3, q23);

	// for each pixel in q12, find its interpolated colour and do putPixel
	Pixel* curr12 = q12->head;
	Pixel* curr13 = q13->head;
	Pixel* curr23 = q23->head;

	Queue* q = malloc(sizeof(Queue));
	//Queue* q23 = malloc(sizeof(Queue));
	//zcurr12 = curr12->next;
	while (curr12 != NULL) {
		// top flat triangle
		// update edge 12
		if (curr12 != NULL) {
			// skip pixels that are on the same y
			while (curr12->next != NULL) {
				if (curr12->next->pt.y == curr12->pt.y)
					curr12 = curr12->next;
				else
					break;
			}
			curr12 = curr12->next;
		} 
		// update edge 13
		if (curr13 != NULL) {
			// skip pixels that are on the same y
			while (curr13->next != NULL) {
				if (curr13->next->pt.y == curr13->pt.y)
					curr13 = curr13->next;
				else
					break;
			}
			curr13 = curr13->next;
		}

		if (curr12 != NULL) {
			gfx_line_bres_col(curr12, curr13, q);
			queue_del(q);
		}
	}
	while ((curr23 != NULL) && (curr13 != NULL)) {
		// top flat triangle
		// update edge 23
		if (curr23 != NULL) {
			// skip pixels that are on the same y
			while (curr23->next != NULL) {
				if (curr23->next->pt.y == curr23->pt.y)
					curr23 = curr23->next;
				else
					break;
			}
			curr23 = curr23->next;
		}
		// update edge 13
		if (curr13 != NULL) {
			// skip pixels that are on the same y
			while (curr13->next != NULL) {
				if (curr13->next->pt.y == curr13->pt.y)
					curr13 = curr13->next;
				else
					break;
			}
			curr13 = curr13->next;
		}
		if ((curr23 != NULL) && (curr13 != NULL)) {
			gfx_line_bres_col(curr23, curr13, q);
			queue_del(q);
		}
	}
}

/* Triangle fill by line sweep: https://www.cs.princeton.edu/courses/archive/fall00/cs426/lectures/scan/sld013.htm, https://www.youtube.com/watch?v=MIW3ljGisak */ 
void gfx_triangle_fill_sweep(vec2i_t* pt1, vec2i_t* pt2, vec2i_t* pt3) {
	float x1 = pt1->x, y1 = pt1->y;
	float x2 = pt1->x, y2 = pt1->y;
	float x3 = pt1->x, y3 = pt1->y;
	float slopeInv13 = (float) (x3 - x1)/(y3 - y1);
	float slopeInv12 = (float) (x2 - x1)/(y2 - y1);
	float slopeInv23 = (float) (x3 - x2)/(y3 - y2);
	float xl = x1, xr = x1;
	for (int y = y1; y < y2; y++) {
		for (int x = (int)xl; x < (int)xr; x++) {
			gfx_point(x, y);
		}
		xl += slopeInv13;
		xr += slopeInv12;
	}
	for (int y = y2; y < y3; y++) {
		for (int x = xl; x < (int)xr; x++) {
			gfx_point(x, y);
		}
		xl += slopeInv13;
		xr += slopeInv23;
	}
}


/* Testing whether a point (x, y) is inside a triangle (pt1, pt2, pt3) */
bool is_interior(int x1, int y1, int x2, int y2, int x3, int y3, int x, int y) {
	int pp1_x = x1 - x;
	int pp1_y = y1 - y;
	int pp2_x = x2 - x;
	int pp2_y = y2 - y;
	int pp3_x = x3 - x;
	int pp3_y = y3 - y;
	return 
	// cw case
	( ((PERP_DOT(pp1_x, pp1_y, pp2_x, pp2_y) < 0) &&
	(PERP_DOT(pp2_x, pp2_y, pp3_x, pp3_y) < 0) &&
	(PERP_DOT(pp3_x, pp3_y, pp1_x, pp1_y) < 0))  ||
	// ccw case
	((PERP_DOT(pp1_x, pp1_y, pp2_x, pp2_y) > 0) &&
	(PERP_DOT(pp2_x, pp2_y, pp3_x, pp3_y) > 0) &&
	(PERP_DOT(pp3_x, pp3_y, pp1_x, pp1_y) > 0)) );
}


/**
 * @brief Fills a triangle given 3 points
 *
 * @param pt1 Vertex 1
 * @param pt2 Vertex 2
 * @param pt3 Vertex 3
 */
void gfx_triangle_fill_int_test(vec2i_t* pt1, vec2i_t* pt2, vec2i_t* pt3) {
	// Ensure y1 <= y2 <= y3 
	if (pt1->y > pt3->y)
		SWAP(*pt1, *pt3);
	if (pt1->y > pt2->y)
		SWAP(*pt1, *pt2);
	if (pt2->y > pt3->y)
		SWAP(*pt2, *pt3);
	// scan the bounding box of the triangle
	int x1 = pt1->x, x2 = pt2->x, x3 = pt3->x;
	int y1 = pt1->y, y2 = pt2->y, y3 = pt3->y;
	int xmin = MIN(MIN(x1, x2), x3);
	int xmax = MAX(MAX(x1, x2), x3);
	for (int y = y1; y < y3; ++y) {
		for (int x = xmin; x < xmax; ++x) {
			if (is_interior(x1, y1, x2, y2, x3, y3, x, y)) { gfx_point(x, y); }
		}
	}
}


/**
 * @brief	Draws a circle using midpoint algorithm
 * 			Explanation: https://github.com/0xLeo/journal/blob/master/graphics/shape_raster/pdf/shape_raster.pdf	
 *
 * @param x0 x-coordinate of centre
 * @param y0 y-coordinate of centre
 * @param r radius
 */
void gfx_circle(int x0, int y0, int r) {
	int x = 0;
	int y = r;
	int dec = 1 - r;
	do {
		gfx_point(x + x0, y + y0);		// octant 2
		gfx_point(y + x0, x + y0);		// 1
		gfx_point(-x + x0, y + y0);		// 3
		gfx_point(-y + x0, x + y0);		// 4
		gfx_point(-y + x0, -x + y0);	// 5
		gfx_point(-x + x0, -y + y0);	// 6
		gfx_point(x + x0, -y + y0);		// 7
		gfx_point(y + x0, -x + y0);		// 8
		dec += 2*x + 3;
		if (dec >= 0)
			dec += - 2*(--y) + 2;
	} while (++x < y);
}


/* Draw a line from (x1,y1) to (x2,y2) */
void gfx_line( int x1, int y1, int x2, int y2 )
{
	XDrawLine(gfx_display, gfx_window, gfx_gc, x1, y1, x2, y2);
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
		XAllocColor(gfx_display, gfx_colormap, &color);
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
		if(XCheckMaskEvent(gfx_display, -1, &event)) {
			if(event.type == KeyPress) {
				XPutBackEvent(gfx_display, &event);
				return 1;
			} else if (event.type == ButtonPress) {
				XPutBackEvent(gfx_display, &event);
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
		XNextEvent(gfx_display, &event);

		if(event.type == KeyPress) {
			saved_xpos = event.xkey.x;
			saved_ypos = event.xkey.y;
			return XLookupKeysym(&event.xkey,0);
		} else if(event.type == ButtonPress) {
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



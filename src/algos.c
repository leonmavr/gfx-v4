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


static inline void vec3u_add(vec3u8_t* result, vec3u8_t* vec1, vec3u8_t* vec2) {
	result->x = vec1->x + vec2->x;	
	result->y = vec1->y + vec2->y;	
	result->z = vec1->z + vec2->z;	
}


static inline void vec3u_mul_float(vec3u8_t* result, vec3u8_t* vec1, float fl) {
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
				pt.point.x = x;
				pt.point.y = y;
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
				pt.point.x = x;
				pt.point.y = y;
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
				pt.point.x = x;
				pt.point.y = y;
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
				pt.point.x = x;
				pt.point.y = y;
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
				pt.point.x = x;
				pt.point.y = y;
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
				pt.point.x = x;
				pt.point.y = y;
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
				pt.point.x = x;
				pt.point.y = y;
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
				pt.point.x = x;
				pt.point.y = y;
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
					pt.point.x = x;
					pt.point.y = y;
					queue_append(q, &pt);
				}
			} else {
				for (y = y2; y < y1; y++) {
					gfx_point(x, y);	
					pt.point.x = x;
					pt.point.y = y;
					queue_append(q, &pt);
				}
			}
			break;
	}
}


void gfx_line_bres_col(Pixel* pt1, Pixel* pt2, Queue* q) {
	gfx_line_bres(&pt1->point, &pt2->point, q);
	int len = queue_length(q);
	vec3u8_t col1 = pt1->colour;
	vec3u8_t col2 = pt2->colour;
	vec3u8_t col1_curr = col1;
	vec3u8_t col2_curr = col2;
	vec3u8_t col_curr;
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
		gfx_point(curr->point.x, curr->point.y);
		gfx_flush();
		curr = curr->next;
	}
}


/**
 * @brief 	Draws a triangle whose every pixel is coloured based on the distance from
 * 			each vertex.	
 *
 * @param pt1 A Pixel ((x, y), (r, g, ,b)) 
 * @param pt2 A Pixel ((x, y), (r, g, ,b)) 
 * @param pt3 A Pixel ((x, y), (r, g, ,b)) 
 * @param q12 A Pixel ((x, y), (r, g, ,b)) 
 * @param q13 A Pixel ((x, y), (r, g, ,b)) 
 * @param q23 A Pixel ((x, y), (r, g, ,b)) 
 */
void gfx_triangle_fill_bres(Pixel* pt1, Pixel* pt2, Pixel* pt3,
		Queue* q12, Queue* q13, Queue* q23) {
	// Ensure y1 <= y2 <= y3 
	if (pt1->point.y > pt3->point.y)
		SWAP(*pt1, *pt3);
	if (pt1->point.y > pt2->point.y)
		SWAP(*pt1, *pt2);
	if (pt2->point.y > pt3->point.y)
		SWAP(*pt2, *pt3);

	gfx_line_bres_col(pt1, pt2, q12);
	gfx_line_bres_col(pt1, pt3, q13);
	gfx_line_bres_col(pt2, pt3, q23);

	// for each pixel in q12, find its interpolated colour and do putPixel
	Pixel* curr12 = q12->head;
	Pixel* curr13 = q13->head;
	Pixel* curr23 = q23->head;

	Queue* q = malloc(sizeof(Queue));
	while (curr12 != NULL) {
		// top flat triangle
		// update edge 12
		if (curr12 != NULL) {
			// skip pixels that are on the same y
			while (curr12->next != NULL) {
				if (curr12->next->point.y == curr12->point.y)
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
				if (curr13->next->point.y == curr13->point.y)
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
		// bottom flat triangle
		// update edge 23
		if (curr23 != NULL) {
			// skip pixels that are on the same y
			while (curr23->next != NULL) {
				if (curr23->next->point.y == curr23->point.y)
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
				if (curr13->next->point.y == curr13->point.y)
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
void gfx_triangle_fill_int_test(Pixel* pt1, Pixel* pt2, Pixel* pt3) {
	// Ensure y1 <= y2 <= y3 
	if (pt1->point.y > pt3->point.y)
		SWAP(*pt1, *pt3);
	if (pt1->point.y > pt2->point.y)
		SWAP(*pt1, *pt2);
	if (pt2->point.y > pt3->point.y)
		SWAP(*pt2, *pt3);
	// scan the bounding box of the triangle
	int x1 = pt1->point.x, x2 = pt2->point.x, x3 = pt3->point.x;
	int y1 = pt1->point.y, y2 = pt2->point.y, y3 = pt3->point.y;
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



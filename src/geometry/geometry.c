#include <math.h>
#include <stdio.h>
#include "queue.h" 

#define PI 3.1415926535
#define DEG2RAD(DEG) ((DEG) * PI / 180.0)
#define RAD2DEG(RAD) ((RAD) * 180.0 / PI)


void geo_point_print(vec2i_t pt) {
	printf("(%u, %u) ", pt.x, pt.y);
}

void geo_rotate(vec2i_t* pt, float deg) {
	int x = pt->x, y = pt->y;
	// TODO: precompute sine at 0-90 deg to optimise
	int f_xNew = round(x*cos(DEG2RAD(deg)) + y*sin(DEG2RAD(deg)));
	int f_yNew = round(-x*sin(DEG2RAD(deg)) + y*cos(DEG2RAD(deg)));
	pt->x = f_xNew; // cast to uint
	pt->y = f_yNew; // cast to uint
}


void geo_shift(vec2i_t* pt, int x, int y){
	pt->x += x;
	pt->y += y;
}

void geo_rotateTriangle(vec2i_t* pt1, vec2i_t* pt2, vec2i_t* pt3, float theta) {
	int centroidX = (int) (pt1->x + pt2->x + pt3->x)/3;
	int centroidY = (int) (pt1->y + pt2->y + pt3->y)/3;

	geo_shift(pt1, -centroidX, -centroidY);
	geo_shift(pt2, -centroidX, -centroidY);
	geo_shift(pt3, -centroidX, -centroidY);

	geo_rotate(pt1, theta);
	geo_rotate(pt2, theta);
	geo_rotate(pt3, theta);

	geo_shift(pt1, +centroidX, +centroidY);
	geo_shift(pt2, +centroidX, +centroidY);
	geo_shift(pt3, +centroidX, +centroidY);
}

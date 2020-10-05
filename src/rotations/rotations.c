#include "queue.h"
#include "gfx.h"
#include <math.h>
#include <stdio.h>

#define PI 3.1415926535
#define DEG2RAD(DEG) ((DEG) * PI / 180.0)
#define RAD2DEG(RAD) ((RAD) * 180.0 / M_PI)

static void rotate(Point* pt, float deg) {
	int x = pt->x, y = pt->y, xNew, yNew;
	float f_xNew = round(x*cos(DEG2RAD(deg)) + y*sin(DEG2RAD(deg)));
	float f_yNew = round(-x*sin(DEG2RAD(deg)) + y*cos(DEG2RAD(deg)));
	pt->x = f_xNew; // cast to uint
	pt->y = f_yNew; // cast to uint
	pt->valid = 1 ? ((f_xNew < 0) || (f_yNew < 0)) : 0;
}


static void shift(Point* pt, int x, int y){
	pt->x += x;
	pt->y += y;
}

void rotateTriangle(Point* pt1, Point* pt2, Point* pt3, float theta) {
	int centroidX = (pt1->x + pt2->x + pt3->x)/3;
	int centroidY = (pt1->y + pt2->y + pt3->y)/3;

	shift(pt1, -centroidX, -centroidY);
	shift(pt2, -centroidX, -centroidY);
	shift(pt3, -centroidX, -centroidY);

	rotate(pt1, theta);
	rotate(pt2, theta);
	rotate(pt3, theta);

	shift(pt1, +centroidX, +centroidY);
	shift(pt2, +centroidX, +centroidY);
	shift(pt3, +centroidX, +centroidY);
}

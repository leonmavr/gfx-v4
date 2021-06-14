#include <math.h>
#include <stdio.h>
#include "queue.h" 

#define PI 3.1415926535
#define DEG2RAD(DEG) ((DEG) * PI / 180.0)
#define RAD2DEG(RAD) ((RAD) * 180.0 / PI)


void geo_rotate(Pixel* px, float deg) {
	int x = px->point.x, y = px->point.y;
	// TODO: precompute sine at 0-90 deg to opximise
	int f_xNew = round(x*cos(DEG2RAD(deg)) + y*sin(DEG2RAD(deg)));
	int f_yNew = round(-x*sin(DEG2RAD(deg)) + y*cos(DEG2RAD(deg)));
	px->point.x = f_xNew; // cast to uint
	px->point.y = f_yNew; // cast to uint
}

void geo_rotateAboutAnohter(Pixel* current, Pixel* other, float deg) {
	int cx = current->point.x, cy = current->point.y;
	int ox = other->point.x, oy = other->point.y;
	current->point.x = ox + cos(deg) * (cx - ox) - sin(deg) * (cy - oy);
    current->point.y = oy + sin(deg) * (cx - ox) + cos(deg) * (cy - oy);
}


void geo_shift(Pixel* px, int x, int y){
	px->point.x += x;
	px->point.y += y;
}

void geo_rotateTriangle(Pixel* px1, Pixel* px2, Pixel* px3, float theta) {
	int centroidX = (int) (px1->point.x + px2->point.x + px3->point.x)/3;
	int centroidY = (int) (px1->point.y + px2->point.y + px3->point.y)/3;

	geo_shift(px1, -centroidX, -centroidY);
	geo_shift(px2, -centroidX, -centroidY);
	geo_shift(px3, -centroidX, -centroidY);

	geo_rotate(px1, theta);
	geo_rotate(px2, theta);
	geo_rotate(px3, theta);

	geo_shift(px1, +centroidX, +centroidY);
	geo_shift(px2, +centroidX, +centroidY);
	geo_shift(px3, +centroidX, +centroidY);
}

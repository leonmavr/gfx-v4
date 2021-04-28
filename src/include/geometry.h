#ifndef GEOMETRY_H
#define GEOMETRY_H 

typedef struct point_t {
	unsigned int x;
	unsigned int y;
	unsigned int valid;
} Point;


typedef struct vec2i {
	int x;
	int y;
} vec2i;


void geo_point_print(Point pt);
void geo_rotate(Point* pt, float deg);
void geo_shift(Point* pt, int x, int y);
void geo_rotateTriangle(Point* pt1, Point* pt2, Point* pt3, float theta);

#endif /* GEOMETRY_H */

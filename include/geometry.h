#ifndef GEOMETRY_H
#define GEOMETRY_H 


typedef struct vec2i {
	int x;
	int y;
} vec2i;


void geo_point_print(vec2i pt);
void geo_rotate(vec2i* pt, float deg);
void geo_shift(vec2i* pt, int x, int y);
void geo_rotateTriangle(vec2i* pt1, vec2i* pt2, vec2i* pt3, float theta);

#endif /* GEOMETRY_H */

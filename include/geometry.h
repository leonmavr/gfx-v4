#ifndef GEOMETRY_H
#define GEOMETRY_H 


typedef struct vec2i_t {
	int x;
	int y;
} vec2i_t;


typedef struct vec3u_t {
	int x;
	int y;
	int z;
} vec3u_t;


void geo_point_print(vec2i_t pt);
void geo_rotate(vec2i_t* pt, float deg);
void geo_shift(vec2i_t* pt, int x, int y);
void geo_rotateTriangle(vec2i_t* pt1, vec2i_t* pt2, vec2i_t* pt3, float theta);

#endif /* GEOMETRY_H */

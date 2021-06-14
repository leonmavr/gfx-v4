#ifndef GEOMETRY_H
#define GEOMETRY_H 


void geo_rotate(Pixel* pt, float deg);
void geo_rotateAboutAnother(Pixel* current, Pixel* other, float deg);
void geo_shift(Pixel* pt, int x, int y);
void geo_rotateTriangle(Pixel* pt1, Pixel* pt2, Pixel* pt3, float theta);

#endif /* GEOMETRY_H */

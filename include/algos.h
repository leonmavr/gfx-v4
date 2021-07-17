#ifndef ALGOS_H
#define ALGOS_H 

#include "types.h"

/**
 * @brief Draw a straight line from pt1 to pt2 using Bresenham's algorithm	
 *
 * @param pt1 A 2D vector {x, y} 
 * @param pt2 A 2D vector {x, y} 
 * @param q An allocated Queue. Stores the points of the line from pt1 to pt2 
 */
void algos_line_bres(const vec2i_t* pt1, const vec2i_t* pt2, Queue* q);

/**
 * @brief Draw a colour gradient straight line using Bresenham's
 *
 * @param pt1 A Pixel {{x, y}, {r, g, b}} 
 * @param pt2 A Pixel {{x, y}, {r, g, b}} 
 * @param q An allocated Queue. Stores the points of the line from pt1 to pt2 
 */
void algos_line_bres_col(Pixel* pt1, Pixel* pt2, Queue* q);

/**
 * @brief Draws a multi-colour triangle whose every pixel is coloured based on
 *        the distance from each vertex (colour smearing).
 *
 * @param pt1 A Pixel ((x, y), (r, g, ,b)) 
 * @param pt2 A Pixel ((x, y), (r, g, ,b)) 
 * @param pt3 A Pixel ((x, y), (r, g, ,b)) 
 * @param q12 A Pixel ((x, y), (r, g, ,b)) 
 * @param q13 A Pixel ((x, y), (r, g, ,b)) 
 * @param q23 A Pixel ((x, y), (r, g, ,b)) 
 */
void algos_triangle_fill_bres(Pixel* pt1, Pixel* pt2, Pixel* pt3,
                            Queue* q12, Queue* q13, Queue* q23);

/**
 * @brief Draw a solid triangle using the line sweep method: 
 * https://www.cs.princeton.edu/courses/archive/fall00/cs426/lectures/scan/sld013.htm
 *
 * @param pt1 A 2D vector {x, y} 
 * @param pt2 A 2D vector {x, y} 
 * @param pt3 A 2D vector {x, y} 
 */
void algos_triangle_fill_sweep(vec2i_t* pt1, vec2i_t* pt2, vec2i_t* pt3);

/**
 * @brief   Draw a solid triangle using the interior test for each point
 *          in its bounding box: 
 *
 * @param pt1 A 2D vector {x, y} 
 * @param pt2 A 2D vector {x, y} 
 * @param pt3 A 2D vector {x, y} 
 */
void algos_triangle_fill_int_test(Pixel* pt1, Pixel* pt2, Pixel* pt3);

/**
 * @brief	Draws a circle using midpoint algorithm
 * 		Explanation: https://github.com/0xLeo/journal/blob/master/graphics/shape_raster/pdf/shape_raster.pdf	
 *
 * @param x0 x-coordinate of centre
 * @param y0 y-coordinate of centre
 * @param r radius
 */
void algos_circle(int x0, int y0, int r);

/**
 * @brief   Draw a Bezier curve using uniformly sampled De Chasteljau algorithm,
 *          given some control points
 *
 * @param pixelArray A pointer to an array of Pixels. These act as the control points
 * @param n The number of control points (length of the above array)
 * @param dt The sampling width, 0<t<1. The smaller, the more detailed the curve
 */
void algos_bezier(const Pixel* pixelArray, const unsigned n, const float dt);

#endif /* ALGOS_H */

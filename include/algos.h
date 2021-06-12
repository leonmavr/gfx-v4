#ifndef ALGOS_H
#define ALGOS_H 


/* Draw line using Bresenham's */
void gfx_line_bres(const vec2i_t* pt1, const vec2i_t* pt2, Queue* q);

/* Draw a colour gradient line using Bresenham's */
void gfx_line_bres_col(Pixel* pt1, Pixel* pt2, Queue* q);

/* Triangle fill with Bresenham with colour interpolated from vertices */
void gfx_triangle_fill_bres(Pixel* pt1, Pixel* pt2, Pixel* pt3,
							Queue* q12, Queue* q13, Queue* q23);
/* Fill triangle with line sweep */
void gfx_triangle_fill_sweep(vec2i_t* pt1, vec2i_t* pt2, vec2i_t* pt3);
/* Fill triangle with interior test */
void gfx_triangle_fill_int_test(vec2i_t* pt1, vec2i_t* pt2, vec2i_t* pt3);

/* Draw a circle of radius r at (x0, y0) */
void gfx_circle(int x0, int y0, int r);

#endif /* ALGOS_H */

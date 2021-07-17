#ifndef TYPES_H
#define TYPES_H

typedef unsigned char u8_t;

typedef struct vec2i_t {
    int x;
    int y;
} vec2i_t;

typedef struct vec3u8_t {
    u8_t x;
    u8_t y;
    u8_t z;
} vec3u8_t;

typedef struct node_t {
    vec2i_t point;
    vec3u8_t colour;
    struct node_t* next;
} Pixel;

#endif /* TYPES_H */

#ifndef QUEUE_H
#define QUEUE_H 

#include "geometry.h" // struct Point


// TODO: rename this to Pixel, rename pt to point
typedef struct node_t {
	vec2i_t pt;
	vec3u_t colour;
    struct node_t* next;
} PointNode;


typedef struct queue_t {
	PointNode* head;
	PointNode* tail;
} Queue;


unsigned int queue_append(Queue* queue, PointNode* pt);
unsigned int queue_append_colour(Queue* queue, vec2i_t* pt, vec3u_t* colour);
unsigned int queue_del(Queue* queue);
void queue_init(Queue* queue);
unsigned int queue_isEmpty(Queue* queue);
vec2i_t queue_pop(Queue* queue);
unsigned int queue_print(Queue* queue);
unsigned int queue_length(Queue* queue);

#endif /* QUEUE_H */

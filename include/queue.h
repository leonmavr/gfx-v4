#ifndef QUEUE_H
#define QUEUE_H 

#include "geometry.h" // struct Point


typedef struct node_t {
	vec2i pt;
    struct node_t* next;
} PointNode;

typedef struct queue_t {
	PointNode* head;
	PointNode* tail;
} Queue;

unsigned int queue_append(Queue* queue, vec2i pt);
unsigned int queue_del(Queue* queue);
void queue_init(Queue* queue);
unsigned int queue_isEmpty(Queue* queue);
vec2i queue_pop(Queue* queue);
unsigned int queue_print(Queue* queue);

#endif /* QUEUE_H */

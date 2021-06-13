#ifndef QUEUE_H
#define QUEUE_H 

#include "types.h"


typedef struct queue_t {
	Pixel* head;
	Pixel* tail;
} Queue;


unsigned int queue_append(Queue* queue, Pixel* pt);
unsigned int queue_append_colour(Queue* queue, vec2i_t* pt, vec3u8_t* colour);
unsigned int queue_del(Queue* queue);
void queue_init(Queue* queue);
unsigned int queue_isEmpty(Queue* queue);
void queue_pop(Pixel* result, Queue* queue);
unsigned int queue_print(Queue* queue);
unsigned int queue_length(Queue* queue);

#endif /* QUEUE_H */

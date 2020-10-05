#ifndef QUEUE_H
#define QUEUE_H 


typedef struct point_t {
	unsigned int x;
	unsigned int y;
	unsigned int valid;
} Point;

typedef struct node_t {
	Point pt;
    struct node_t* next;
} PointNode;

typedef struct queue_t {
	PointNode* head;
	PointNode* tail;
} Queue;

unsigned int queue_append(Queue* queue, Point pt);
unsigned int queue_del(Queue * queue);
void queue_init(Queue* queue);
unsigned int queue_isEmpty(Queue* queue);
Point queue_pop(Queue* queue);
void point_print(Point pt);
unsigned int queue_print(Queue* queue);

#endif /* QUEUE_H */

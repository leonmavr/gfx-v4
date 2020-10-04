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

unsigned int append(Queue* queue, Point pt);
unsigned int del(Queue * queue);
void init(Queue* queue);
unsigned int isEmpty(Queue* queue);
Point pop(Queue* queue);
//unsigned int pop(Queue* queue);
void printPoint(Point pt);
unsigned int printQueue(Queue* queue);

#endif /* QUEUE_H */

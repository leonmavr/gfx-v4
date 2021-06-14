#include <stdlib.h>
#include <stdio.h>
#include "types.h" 
#include "queue.h" 
#include "geometry.h" 


enum {
	NO_ERROR,
	ERROR_CANNOT_ALLOC,
	ERROR_LIST_EMPTY
} ErrorCodes;


/**
 * @brief Sets head and tail to null
 *
 * @param queue A queue to initialise
 */
void queue_init(Queue* queue) {
	queue->head = NULL;
	queue->tail = NULL;
}


unsigned int queue_isEmpty(Queue* queue) {
	return queue->head == queue->tail;
}


/**
 * @brief Add a point at the end (tail) of the queue. 
 *
 * @param queue A Queue
 * @param pt A Point structure
 *
 * @return A positive integer if error, else 0
 */
unsigned int queue_append(Queue* queue, Pixel* pt) {
	if (queue->head == NULL) {
		// add node to empty list
		Pixel* next = malloc(sizeof(Pixel));
		if (next == NULL)
			return ERROR_CANNOT_ALLOC;
		// create next node
		next->point = pt->point;
		next->colour = pt->colour;
		next->next = NULL;
		// initialise list with 1 node
		queue->head = next;
		queue->tail = next; 
	} else {
		// add to tail
		queue->tail->next = malloc(sizeof(Pixel));
		if (queue->tail->next == NULL)
			return ERROR_CANNOT_ALLOC;
		queue->tail->next->next = NULL;
		queue->tail->next->point = pt->point;
		queue->tail->next->colour = pt->colour;
		queue->tail = queue->tail->next;
	}
	return NO_ERROR;
}


/**
 * @brief Remove the last element (at tail)
 *
 * @param queue A Queue
 *
 * @return A valid point (x, y, 1) if queue non empty, else invalid point, i.e. (x, y, 0)
 */
void queue_pop(Pixel* result, Queue* queue) {
	// if head next not null
	Pixel* tmp = queue->head;
	if (tmp != NULL) {
		// get current point
		result = queue->head;
		// move to next and delete current
		queue->head = queue->head->next;
		free(tmp);
	}
}


/**
 * @brief delete (free) all nodes of the Queue
 *
 * @param queue A Queue
 *
 * @return A positive integer if error, else 0
 */
unsigned int queue_del(Queue* queue) {
	// check for errors
	if (queue_isEmpty(queue))
		return ERROR_LIST_EMPTY;
	Pixel* curr = queue->head;
	Pixel* next;
	// do the work
	while (curr != NULL){
		next = curr->next;
		free(curr);
		curr = next;
	} 
	queue->head = NULL;
	return NO_ERROR;
}


/* Count nodes in queue */
unsigned int queue_length(Queue* queue) {
	unsigned length = 0;
	Pixel* curr = queue->head;
	while (curr != NULL) {
		length++;
		curr = curr->next;
	}
	return length;
}

#include <stdlib.h>
#include <stdio.h>
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
unsigned int queue_append_colour(Queue* queue, vec2i_t* pt, vec3u_t* colour) {
	if (queue->head == NULL) {
		// add node to empty list
		PointNode* next = malloc(sizeof(PointNode));
		if (next == NULL)
			return ERROR_CANNOT_ALLOC;
		// create next node
		next->pt = *pt;
		next->colour = *colour;
		next->next = NULL;
		// initialise list with 1 node
		queue->head = queue->tail = next; 
	} else {
		// add to tail
		queue->tail->next = malloc(sizeof(PointNode));
		queue->tail->next->next = NULL;
		queue->tail->next->pt = *pt;
		queue->tail->next->colour = *colour;
		queue->tail = queue->tail->next;
	}
	return NO_ERROR;
}


/**
 * @brief Add a point at the end (tail) of the queue. 
 *
 * @param queue A Queue
 * @param pt A Point structure
 *
 * @return A positive integer if error, else 0
 */
unsigned int queue_append(Queue* queue, PointNode* pt) {
	if (queue->head == NULL) {
		// add node to empty list
		PointNode* next = malloc(sizeof(PointNode));
		if (next == NULL)
			return ERROR_CANNOT_ALLOC;
		// create next node
		next->pt = pt->pt;
		next->colour = pt->colour;
		next->next = NULL;
		// initialise list with 1 node
		queue->head = queue->tail = next; 
	} else {
		// add to tail
		queue->tail->next = malloc(sizeof(PointNode));
		queue->tail->next->next = NULL;
		queue->tail->next->pt = pt->pt;
		queue->tail->next->colour = pt->colour;
		queue->tail = queue->tail->next;
	}
	return NO_ERROR;
}



/**
 * @brief Prints the contents of the queue from head to tail
 *
 * @param queue A Queue
 *
 * @return A positive integer if error, else 0
 */
unsigned int queue_print(Queue* queue) {
	if (queue_isEmpty(queue)) 
		return ERROR_LIST_EMPTY;
	PointNode* iter = queue->head;
	if (iter == NULL)
		return ERROR_CANNOT_ALLOC;
	// do the work
	do { 
		geo_point_print(iter->pt);
		iter  = iter->next;
	} while (iter != NULL);

	free(iter);
	return NO_ERROR;
}


/**
 * @brief Remove the last element (at tail)
 *
 * @param queue A Queue
 *
 * @return A valid point (x, y, 1) if queue non empty, else invalid point, i.e. (x, y, 0)
 */
vec2i_t queue_pop(Queue* queue) {
	// if head next not null
	PointNode* tmp = queue->head;
	vec2i_t ret;
	if (tmp != NULL) {
		// get current point
		ret = queue->head->pt;
		// move to next and delete current
		queue->head = queue->head->next;
		free(tmp);
	} else {
		// empty list - then return invalid point
		ret.x = 0;
		ret.y = 0;
	}
	return ret;
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
	PointNode* curr = queue->head;
	PointNode* next;
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
	PointNode* curr = queue->head;
	while (curr != NULL) {
		length++;
		curr = curr->next;
	}
	return length;
}

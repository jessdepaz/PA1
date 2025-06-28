#ifndef PQUEUE_H__
#define PQUEUE_H__
#include "simulate.h"
typedef struct {
    process* list;
    int size;
    int capacity;
} pqueue;

// Function to initialize the priority queue (min-heap)
pqueue init_pqueue(int capacity);

// Function to add a process to the priority queue
void enqueue_srt(pqueue* pq, process new_process);
void enqueue_sjf(pqueue* pq, process new_process);

// Function to remove the process with the minimum remaining_time
process dequeue_srt(pqueue* pq);
process dequeue_sjf(pqueue* pq);

// Function to peek at the process with the minimum remaining_time without removing it
process pq_peek(pqueue pq) ;

// Function to free the priority queue's allocated memory
void pq_free(pqueue* pq);

char* pq_to_str(pqueue pq);

#endif //PQUEUE_H__
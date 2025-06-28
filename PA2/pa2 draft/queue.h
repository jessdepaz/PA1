#ifndef QUEUE_H__
#define QUEUE_H__

#include "simulate.h"
typedef struct node {
    process value;
    struct node* next;
} node;

typedef struct {
    node* head;
    node* tail;
    int size;
} queue;
queue init_queue();
void enqueue(queue* q, process new_process);
process dequeue(queue* q);
process peek(queue q);
char* to_str(queue q);
#endif /* QUEUE_H__*/
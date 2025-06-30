#include "queue.h"
// Function to initialize a queue
queue init_queue() {
    queue q;
    q.head = NULL;
    q.tail = NULL;
    q.size = 0;
    return q;
}

// Function to enqueue (add an element to the end of the queue)
void enqueue(queue* q, process new_process) {
    node* new_node = (node*)malloc(sizeof(node));
    if (new_node == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    
    new_node->value = new_process;
    new_node->next = NULL;
    
    if (q->tail == NULL) {
        q->head = new_node;
        q->tail = new_node;
    } else {
        q->tail->next = new_node;
        q->tail = new_node;
    }
    
    q->size++;
}

// Function to dequeue (remove an element from the front of the queue)
process dequeue(queue* q) {
    process empty_process = {-1, -1, -1, -1, -1, -1, -1};  // To return in case of an empty queue
    if (q->head == NULL) {
        printf("Queue is empty\n");
        return empty_process;
    }
    
    node* temp = q->head;
    process dequeued_process = temp->value;
    
    q->head = q->head->next;
    if (q->head == NULL) {
        q->tail = NULL;
    }
    
    free(temp);
    q->size--;
    
    return dequeued_process;
}
process peek(queue q) {
    process empty_process = {-1, -1, -1, -1, -1, -1, -1};  // To return in case of an empty queue
    if (q.head == NULL) {
        printf("Queue is empty\n");
        return empty_process;
    }
	return q.head->value;
}
char* to_str(queue q){
	char* rv = (char*) malloc(10000 * sizeof(char));
	strcpy(rv, "front_end [");
	char* temp = rv+strlen(rv);
	node * cur = q.head;
	while (cur != NULL){
		int num = sprintf(temp, " p%d", cur->value.id);
		temp += num;
		cur = cur->next;
	}
	strcat(temp, " ] rear_end");
	return rv;
}

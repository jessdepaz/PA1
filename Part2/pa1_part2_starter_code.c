/*
Part II: Queue Implementation

(25 points) Implement the queue ADT (FIFO) using a linked list in C. To do this, you need 
to implement functions enqueue (for adding an element of type int to the rear end of queue),
 dequeue (for removing an element of type int from the front end of the queue), and size 
 (for returning the number of elements in the queue). Please test your implementation extensively
 to be sure about its correctness. You need to submit the C program containing you test cases along
 the queue implementation.
*/


#include<stdio.h>
#include<stdlib.h>
struct node{
	int data;
	struct node* next;
};
typedef struct node node;
typedef struct{
	node* head;
	node* tail;
	int size;
}queue;

//init_queue: initialize the queue
queue init_queue(){
	queue q;
	q.head = q.tail = NULL;
	q.size = 0;
	return q;
}
//enqueue: add to tail
void enqueue(queue* q, int new_data){
	node* new_node = (node*)malloc(sizeof(node));
	new_node->data = new_data;
	new_node->next = NULL;
	if(q->head == NULL)//q is empty
		q->head = q->tail = new_node;
	else{//q is not empty
		q->tail->next = new_node;
		q->tail = new_node;
	}
	q->size++;
}
//dequeue: remove from head
int dequeue(queue* q);

int size(queue q){
	return q.size;
}

int main(){
	queue test = init_queue();
	for(int i = 0; i < 10;i++)
		enqueue(&test, i);
	while(size(test)!= 0)
		printf("next value is %d.\n", dequeue(&test));
}
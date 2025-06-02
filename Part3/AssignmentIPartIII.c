/*
Part III: Priority Queue Implementation

(25 points) Implement the priority queue ADT using a max binary heap in C.
To do this, you need to implement functions add (for adding an element of type int to the heap),
extract_max (for removing the greatest element stored in the heap),
and size (for returning the number of elements in the heap).
Please test your implementation extensively to be sure about its correctness.
You need to submit the C program containing you test cases along with heap implementation.
 */

#include <stdio.h>
#include <stdlib.h>
typedef struct {
    int data[100];
    int size;
}priority_queue;

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

//init_queue: initialize the queue
priority_queue init_queue() {
    priority_queue pq;
    pq.size = 0;
    return pq;
}

// Add an element of type int to the heap
void add(priority_queue* pq, int value) {
    if (pq->size >= 100) {
        printf("Heap overflow.\n");
        return;
    }
    int index = pq->size;
    pq->data[index] = value;
    pq->size++;
    
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (pq->data[index] > pq->data[parent]){
            swap(&pq->data[index], &pq->data[parent]);
            index = parent;
        } 
        else {
            break;
        }
    }
}

void heapify_down(priority_queue* pq, int index) {
    int largest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < pq->size && pq->data[left] > pq->data[largest])
        largest = left;
    if (right < pq->size && pq->data[right] > pq->data[largest])
        largest = right;

    if (largest != index) {
        swap(&pq->data[index], &pq->data[largest]);
        heapify_down(pq, largest);
    }
}

// remove the greatest element stored in the heap
int extract_max(priority_queue* pq) {
    if (pq->size == 0) {
        printf("Heap underflow.\n");
        return -1;
    }
    int max = pq->data[0];
    pq->data[0] = pq->data[pq->size - 1];
    pq->size--;
    heapify_down(pq, 0);
    return max;
}

// return the number of elements in the heap
int size(priority_queue pq) {
    return pq.size;
}

int main() {
    priority_queue test = init_queue();

    for (int i = 0; i < 10; i++){
        add(&test, i);
    }

    while (size(test) != 0){
        printf("next value is %d.\n", extract_max(&test));
    }
        
    return 0;
}
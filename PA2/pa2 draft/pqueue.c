#include "pqueue.h"
// Function to initialize the priority queue (min-heap)
pqueue init_pqueue(int capacity) {
    pqueue pq;
    pq.list = (process*)malloc(sizeof(process) * capacity);
    pq.size = 0;
    pq.capacity = capacity;
    return pq;
}

// Helper function to swap two processes
static void swap(process* a, process* b) {
    process temp = *a;
    *a = *b;
    *b = temp;
}

// Helper function to heapify up (used in adding a process)
static void heapify_up_srt(pqueue* pq, int index) {
    while (index > 0) {
        int parent_index = (index - 1) / 2;
        if (pq->list[index].remaining_time >= pq->list[parent_index].remaining_time) {
            break;
        }
        swap(&pq->list[index], &pq->list[parent_index]);
        index = parent_index;
    }
}
static void heapify_up_sjf(pqueue* pq, int index) {
    while (index > 0) {
        int parent_index = (index - 1) / 2;
        if (pq->list[index].total_cpu_time >= pq->list[parent_index].total_cpu_time) {
            break;
        }
        swap(&pq->list[index], &pq->list[parent_index]);
        index = parent_index;
    }
}

// Helper function to heapify down (used in removing the minimum element)
static void heapify_down_srt(pqueue* pq, int index) {
    int left, right, smallest;
    while (index < pq->size) {
        left = 2 * index + 1;
        right = 2 * index + 2;
        smallest = index;

        if (left < pq->size && pq->list[left].remaining_time < pq->list[smallest].remaining_time) {
            smallest = left;
        }
        if (right < pq->size && pq->list[right].remaining_time < pq->list[smallest].remaining_time) {
            smallest = right;
        }

        if (smallest == index) {
            break;
        }
        swap(&pq->list[index], &pq->list[smallest]);
        index = smallest;
    }
}
static void heapify_down_sjf(pqueue* pq, int index) {
    int left, right, smallest;
    while (index < pq->size) {
        left = 2 * index + 1;
        right = 2 * index + 2;
        smallest = index;

        if (left < pq->size && pq->list[left].total_cpu_time < pq->list[smallest].total_cpu_time) {
            smallest = left;
        }
        if (right < pq->size && pq->list[right].total_cpu_time < pq->list[smallest].total_cpu_time) {
            smallest = right;
        }

        if (smallest == index) {
            break;
        }
        swap(&pq->list[index], &pq->list[smallest]);
        index = smallest;
    }
}

// Function to add a process to the priority queue
void enqueue_srt(pqueue* pq, process new_process) {
    if (pq->size == pq->capacity) {
        printf("Priority queue is full. Cannot add more processes.\n");
        return;
    }
    pq->list[pq->size] = new_process;
    heapify_up_srt(pq, pq->size);
    pq->size++;
}
void enqueue_sjf(pqueue* pq, process new_process) {
    if (pq->size == pq->capacity) {
        printf("Priority queue is full. Cannot add more processes.\n");
        return;
    }
    pq->list[pq->size] = new_process;
    heapify_up_sjf(pq, pq->size);
    pq->size++;
}

// Function to remove the process with the minimum remaining_time
process dequeue_srt(pqueue* pq) {
    if (pq->size == 0) {
        printf("Priority queue is empty. Returning a default process.\n");
		process empty_process = {-1, -1, -1, -1, -1, -1, -1};  // To return in case of an empty queue
        return empty_process;  // Return a default process with id 0
    }
    process min_process = pq->list[0];
    pq->list[0] = pq->list[pq->size - 1];
    pq->size--;
    heapify_down_srt(pq, 0);
    return min_process;
}
process dequeue_sjf(pqueue* pq) {
    if (pq->size == 0) {
        printf("Priority queue is empty. Returning a default process.\n");
		process empty_process = {-1, -1, -1, -1, -1, -1, -1};  // To return in case of an empty queue
        return empty_process;  // Return a default process with id 0
    }
    process min_process = pq->list[0];
    pq->list[0] = pq->list[pq->size - 1];
    pq->size--;
    heapify_down_sjf(pq, 0);
    return min_process;
}

// Function to peek at the process with the minimum remaining_time without removing it
process pq_peek(pqueue pq) {
    if (pq.size == 0) {
        printf("Priority queue is empty. Returning a default process.\n");
		process empty_process = {-1, -1, -1, -1, -1, -1, -1};  // To return in case of an empty queue
        return empty_process;  // Return a default process with id 0
    }
    return pq.list[0];
}

// Function to free the priority queue's allocated memory
void pq_free(pqueue* pq) {
    free(pq->list);
    pq->list = NULL;
    pq->size = 0;
    pq->capacity = 0;
}

char* pq_to_str(pqueue pq){
	char* rv = (char*) malloc(10000 * sizeof(char));
	strcpy(rv, "front_end [");
	char* temp = rv+strlen(rv);
	for(int i = 0; i < pq.size;i++){
		int num = sprintf(temp, " p%d", pq.list[i].id);
		temp += num;
	}
	strcat(temp, " ] rear_end");
	return rv;
}
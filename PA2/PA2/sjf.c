#include "simulate.h"
#include "pqueue.h"
#define BUSY 1
#define FREE 0

void simulate_sjf() {
    int completed = 0;
	int t = 0;
	int cpu_status = FREE;
	pqueue ready = init_pqueue(MAX_PROCESSES);

    while (completed < n) {
		// Check for new arrivals and enqueue
		for (int i = 0; i < n; i++) {
			if (processes[i].arrival_time == t){ 
				processes[i].active = 1;
				enqueue_sjf(&ready, processes[i]);
			}
		}

        if (!ready.size && cpu_status == FREE) {
            t++;
            continue;
        }
		int cur;

		// If CPU is free, dequeue the next process
		if(cpu_status == FREE){
			cur= dequeue_sjf(&ready).id;
			cpu_status = BUSY;
		}
        processes[cur].remaining_time--;
        t++;

        if (processes[cur].remaining_time == 0) {
            processes[cur].active = 0;
            processes[cur].turnaround_time = t - processes[cur].arrival_time;
            completed++;
			cpu_status = FREE;
        }
    }
}
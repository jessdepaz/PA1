#include "simulate.h"
#include "queue.h"
void simulate_fcfs() {
    int t = 0;
    int completed = 0;
	queue ready = init_queue();

    while (completed < n) {
		// Enqueue newly arrived processes
		for (int i = 0; i < n; i++) {
			if (processes[i].arrival_time == t){ 
				processes[i].active = 1;
				enqueue(&ready, processes[i]);
			}
		}
		// If no active process is in the ready queue, increment time
        if (!ready.size) {
            t++;
            continue;
        }
		// Run the process at the front of the queue
		int cur = peek(ready).id;
        processes[cur].remaining_time--;
        t++;
		// If process finishes
        if (processes[cur].remaining_time == 0) {
            processes[cur].active = 0;
            processes[cur].turnaround_time = t - processes[cur].arrival_time;
            completed++;
			dequeue(&ready);
        }
    }
}
#include "simulate.h"
#include "pqueue.h"
#define BUSY 1
#define FREE 0
void simulate_sjf() {
    int t = 0;
    int completed = 0;
	pqueue ready = init_pqueue(MAX_PROCESSES);
	int cpu_status = FREE;
    while (completed < n) {
		//char* str = pq_to_str(ready);
		//printf("Ready queue @ t = %d: %s\n", t, str);
		//free(str);
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
			//printf("p%d arrived at %d, got terminated at %d, with TT = %d.\n",
			//	processes[cur].id, processes[cur].arrival_time,
			//	t, processes[cur].turnaround_time);
        }
    }
}
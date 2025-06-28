#include "simulate.h"
#include "queue.h"
void simulate_fcfs() {
    int t = 0;
    int completed = 0;
	//queue ready[10];
	//for(int i = 0; i < 10;i++)
		//ready[i] = init_queue();
	queue ready = init_queue();
    while (completed < n) {
		//char* str = to_str(ready);
		//printf("Ready queue @ t = %d: %s\n", t, str);
		//free(str);
		for (int i = 0; i < n; i++) {
			if (processes[i].arrival_time == t){ 
				processes[i].active = 1;
				enqueue(&ready, processes[i]);
			}
		}
        if (!ready.size) {
            t++;
            continue;
        }
		int cur = peek(ready).id;
        processes[cur].remaining_time--;
        t++;
        if (processes[cur].remaining_time == 0) {
            processes[cur].active = 0;
            processes[cur].turnaround_time = t - processes[cur].arrival_time;
            completed++;
			dequeue(&ready);
			//printf("p%d arrived at %d, got terminated at %d, with TT = %d.\n",
			//	processes[cur].id, processes[cur].arrival_time,
			//	t, processes[cur].turnaround_time);
        }
    }
}
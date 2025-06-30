#include "simulate.h"
#include "pqueue.h"
#define FREE 0
#define BUSY 1

void simulate_srt() {
    int t = 0;
    int completed = 0;
    int cpu_status = FREE;
    int cur = -1;

    pqueue ready = init_pqueue(MAX_PROCESSES);

    while (completed < n) {
        // Add newly arrived processes
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time == t) {
                processes[i].active = 1;
                enqueue_srt(&ready, processes[i]);
            }
        }

        // If no process is ready, time moves forward
        if (ready.size == 0 && cpu_status == FREE) {
            t++;
            continue;
        }

        // Peek top process to decide on preemption
        if (ready.size > 0) {
            process top = pq_peek(ready);

            // If no process is running or the top process has shorter time
            if (cpu_status == FREE || (cur >= 0 && processes[cur].remaining_time > top.remaining_time)) {
                if (cpu_status == BUSY && processes[cur].remaining_time > 0) {
                    enqueue_srt(&ready, processes[cur]);  // preempt and push current back
                }

                process next = dequeue_srt(&ready);
                cur = next.id;
                cpu_status = BUSY;
            }
        }

        // Run the current process
        if (cpu_status == BUSY && cur >= 0) {
            processes[cur].remaining_time--;
            
            if (processes[cur].remaining_time == 0) {
                processes[cur].active = 0;
                processes[cur].turnaround_time = t + 1 - processes[cur].arrival_time;
                completed++;
                cpu_status = FREE;
                cur = -1;
            }
        }

        t++;
    }
}

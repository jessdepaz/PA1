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

    /* repeat until Rᵢ == 0 for all n processes      repeat until all processes have terminated */
    while (completed < n) {
        
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time == t) {
                processes[i].active = 1;
                enqueue_srt(&ready, processes[i]);
            }
        }

        /* while no process is active, increment t */
        if (!ready.size) {
            t++;
            continue;
        }

        /* choose active processes pᵢ to run next according to SRT */
        if (ready.size > 0) {
            process top = pq_peek(ready);

            if (cpu_status == FREE || processes[cur].remaining_time > top.remaining_time) {
                if (cpu_status == BUSY && processes[cur].remaining_time > 0) {
                    enqueue_srt(&ready, processes[cur]);
                }

                process next = dequeue_srt(&ready);
                cur = next.id;
                cpu_status = BUSY;
            }
        } /* else {
             cpu_status = FREE;
             cur = -1;
         } */

        /* decrement Rᵢ                              /* pᵢ has accumulated 1 CPU time unit */
        if (cpu_status == BUSY){
            processes[cur].remaining_time--;

            /* if Rᵢ == 0                                 process i has terminated */
            if (processes[cur].remaining_time == 0) {
                /*  set active flag of pᵢ = 0             process i is excluded from further consideration */

                processes[cur].active = 0;

                /*  TTᵢ = t - Aᵢ  */
                processes[cur].turnaround_time = t + 1 - processes[cur].arrival_time;
                completed++;
                cpu_status = FREE;
                cur = -1;
            }
        }

        t++;
    }
}
#include "simulate.h"
#include "queue.h"

#define PRIORITY_LEVELS 10

void simulate_priority_rr() {
    queue priority_queues[PRIORITY_LEVELS];
    int completed = 0, current_time = 0;

    // Initialize priority queues
    for (int i = 0; i < PRIORITY_LEVELS; i++)
        priority_queues[i] = init_queue();

    // To track which processes have been added to the queues
    int added[MAX_PROCESSES] = {0};

    while (completed < n) {
        // Add newly arrived processes to their respective queues
        for (int i = 0; i < n; i++) {
            if (!added[i] && processes[i].arrival_time <= current_time) {
                enqueue(&priority_queues[processes[i].priority - 1], processes[i]);
                added[i] = 1;
            }
        }

        int executed = 0;
        for (int p = 0; p < PRIORITY_LEVELS; p++) {
            if (priority_queues[p].size > 0) {
                process current = dequeue(&priority_queues[p]);

                int slice = (current.remaining_time < q) ? current.remaining_time : q;
                current_time += slice;
                current.remaining_time -= slice;

                // Update newly arrived processes during execution slice
                for (int i = 0; i < n; i++) {
                    if (!added[i] && processes[i].arrival_time <= current_time) {
                        enqueue(&priority_queues[processes[i].priority - 1], processes[i]);
                        added[i] = 1;
                    }
                }

                if (current.remaining_time == 0) {
                    processes[current.id].turnaround_time = current_time - processes[current.id].arrival_time;
                    completed++;
                } else {
                    enqueue(&priority_queues[p], current); // Re-enqueue at the same level
                }

                executed = 1;
                break; // Preemption: do not execute lower priority levels
            }
        }

        if (!executed)
            current_time++; // Idle CPU if no process has arrived yet
    }
}

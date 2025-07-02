#ifndef SIMULATE_H__
#define SIMULATE_H__
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define MAX_PROCESSES 1000
#define frand() (rand()/(double)RAND_MAX)
#define nrand() (sqrt(-2*log(frand()))*cos(2*M_PI*frand()))
typedef struct {
    int id;
    int arrival_time;
    int total_cpu_time;
    int remaining_time;
    int active;
    int turnaround_time;
    int priority;
} process;

extern process processes[MAX_PROCESSES];
extern int n, k, d, v, q;  // Simulation parameters
void simulate_fcfs();

void simulate_sjf();

void simulate_srt();

void simulate_priority_rr();
#endif
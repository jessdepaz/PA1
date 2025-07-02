#include"simulate.h"
process processes[MAX_PROCESSES];
int n, k, d, v, q;  // Simulation parameters
static void initialize_processes() {
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        processes[i].id = i;
        processes[i].arrival_time = rand() % (k + 1);
        while ((processes[i].total_cpu_time = (int)(nrand() * v + d)) < 1); 
        processes[i].remaining_time = processes[i].total_cpu_time;
        processes[i].active = (processes[i].arrival_time == 0) ? 1 : 0;
        processes[i].turnaround_time = 0;
        processes[i].priority = rand() % 10 + 1; // For priority scheduling
    }
}
static void reset_processes(){
	for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].total_cpu_time;
        processes[i].active = (processes[i].arrival_time == 0) ? 1 : 0;
        processes[i].turnaround_time = 0;
    }
}
static void compute_average_turnaround_time() {
    long total_turnaround_time = 0;

    for (int i = 0; i < n; i++) {
        total_turnaround_time += processes[i].turnaround_time;
    }

    double average_turnaround_time = (double)total_turnaround_time / n;
    fprintf(stdout, "%.2f\t", average_turnaround_time);
}

int main2(int argc, char *argv[]) {
    if (argc != 11) {
        fprintf(stderr, "Usage: %s %s %s %s\n", 
			argv[0], 
			"-n <num_processes> -k <arrival_interval>", 
			"-d <mean_cpu_time> -v <stddev_cpu_time>", 
			"-q <time_quantum>");
        return 1;
    }
	for (int i = 1; i < argc; i += 2) {
        if (argv[i][1] == 'n') 
			n = atoi(argv[i+1]);
        else if (argv[i][1] == 'k') 
			k = atoi(argv[i+1]);
        else if (argv[i][1] == 'd') 
			d = atoi(argv[i+1]);
        else if (argv[i][1] == 'v') 
			v = atoi(argv[i+1]);
        else if (argv[i][1] == 'q') 
			q = atoi(argv[i+1]);
    }
	initialize_processes();

    //Starting FCFS Simulation...
	fprintf(stderr, "FCFS ");
    simulate_fcfs();
    compute_average_turnaround_time();

	reset_processes();
    //Starting SJF Simulation...
	fprintf(stderr, "SJF ");
    simulate_sjf();
    compute_average_turnaround_time();

	reset_processes();
    //Starting SRT Simulation...
	fprintf(stderr, "SRT ");
    simulate_srt();
    compute_average_turnaround_time();

	reset_processes();
    //Starting Priority with Round Robin Simulation...
	fprintf(stderr, "PRI_RR ");
    simulate_priority_rr();
    compute_average_turnaround_time();

    return 0;
}
int main(int argc, char* argv[]){
	int n_and_k[4][2] = {{100,1000}, {500,10000}, {500,5000}, {1000, 10000}};
	char* argv2[12] = {"simulate",
		"-n", (char*) malloc(10*sizeof(char)), 
		"-k", (char*) malloc(10*sizeof(char)), 
		"-d", (char*) malloc(10*sizeof(char)), 
		"-v", (char*) malloc(10*sizeof(char)), 
		"-q", (char*) malloc(10*sizeof(char)), 
		NULL
	};
	for(int scenario = 0; scenario < ((argc == 2)?atoi(argv[1]):4); scenario++){
		fprintf(stderr, "\n\nn = %d, k = %d:\n\n",
			n = n_and_k[scenario][0],
			k = n_and_k[scenario][1]);
		for(int d = k/n; d <= 25*k/n; d += (k/n)){
			int v = d/4, q = d+v;
			fprintf(stderr, "\nd = %d, v = %d, q = %d: ", d, v, q);
			printf("\n%d\t%d\t%d\t%d\t%d\t", n, k, d, v, q);
			sprintf(argv2[2], "%d", n);
			sprintf(argv2[4], "%d", k);
			sprintf(argv2[6], "%d", d);
			sprintf(argv2[8], "%d", v);
			sprintf(argv2[10], "%d", q);
			main2(11,argv2);
		}
		
	}
}

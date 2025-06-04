#ifndef MY_SHELL_H
#define MY_SHELL_H

#include <sys/types.h>   // for pid_t
#include <signal.h>      // for signal‐handler prototype

#define DEFAULT_TIME_LIMIT 5

// The forked child’s pid, used by handle_alarm()
extern pid_t child_pid;

// Execute a single command-line command with a time limit
void execute(char *command, int default_time_limit);

// SIGALRM handler: kills the child if it’s still running
void handle_alarm(int sig);

#endif
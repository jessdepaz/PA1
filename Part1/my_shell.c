/*

Part I: Inter-Process Communication (IPC)

Change the provided starter code so that it behaves like a shell: 
The program either gets no CLAs, or one CLA specifying the default time-limit (in seconds) 
to wait for the child. The program needs to accept an infinite sequence of commands from user. 
Each command is either equal to "quit", which means the user wants to terminate the shell, 
or it follows the following format:

[-t TIME_LIMIT] executable_name [optional_arg1] [optional_arg2] ...

The program must react to this command by running the given executable file with the given sequence of arguments. 
If the command starts with a "-t TIME_LIMIT" (e.g. -t 2), the program must kill the child if the TIME_LIMIT 
is violated (e.g. after 2 seconds). If no TIME_LIMIT is given via the command, 
the default time limit is either 5 seconds, or its equal to the only command-line argument passed 
to the parent program at the time of execution.

*/

#include "my_shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#define DEFAULT_TIME_LIMIT 5
pid_t child_pid;

// Prototypes for execute and alarm handler
void execute(char *command, int default_time_limit);
void handle_alarm(int sig);

// main function
int main(int argc, char *argv[]) {
    int default_time_limit = DEFAULT_TIME_LIMIT;
    if (argc == 2) {
        default_time_limit = atoi(argv[1]);
        if (default_time_limit <= 0) {
            fprintf(stderr, "Usage: %s [positive‐integer‐time_limit]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    char command[1024];  // Buffer for each line of input
    while (1) {
        printf("my shell> ");
        fflush(stdout);

        if (!fgets(command, sizeof(command), stdin)) {
            // EOF or read error: exit the shell 
            putchar('\n');
            break;
        }

        // If user typed just "quit\n", we exit
        if (strcmp(command, "quit\n") == 0) {
            break;
        }

        execute(command, default_time_limit);
    }

    return 0;
}

void execute(char *command, int default_time_limit) {
    char *args[100];        // To store pointers to each token
    int arg_count = 0;      // How many tokens we found
    int time_limit = default_time_limit;

    // Strip trailing newline, if any
    command[strcspn(command, "\n")] = '\0';

    // Tokenize by whitespace
    char *token = strtok(command, " \t");
    if (token == NULL) {
        return;  // empty line
    }

    // Check if first token is "-t"
    if (strcmp(token, "-t") == 0) {
        token = strtok(NULL, " \t");
        if (token != NULL) {
            time_limit = atoi(token);
            if (time_limit <= 0) {
                fprintf(stderr, "Invalid time limit: %s\n", token);
                return;
            }
            token = strtok(NULL, " \t");
        }
    }

    // Build args[] array
    while (token != NULL && arg_count < (int)(sizeof(args)/sizeof(args[0])) - 1) {
        args[arg_count++] = token;
        token = strtok(NULL, " \t");
    }
    args[arg_count] = NULL;

    if (arg_count == 0) {
        return;
    }

    child_pid = fork();
    if (child_pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        // Child process
        execvp(args[0], args);
        perror("execvp");
        _exit(EXIT_FAILURE);
    } else {
        // Parent process
        // Install a basic SIGALRM handler
        signal(SIGALRM, handle_alarm);

        // Start the timer
        alarm(time_limit);

        int status;
        waitpid(child_pid, &status, 0);

        // Cancel alarm if child finished normally
        alarm(0);

        if (WIFEXITED(status)) {
            printf("Child process exited with status %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Child process terminated by signal %d\n", WTERMSIG(status));
        }
    }
}

// alarm handler function
void handle_alarm(int sig) {
    if (child_pid > 0) {
        kill(child_pid, SIGKILL);
        printf("Child process timed out and was terminated.\n");
    }
}// end handle_alarm
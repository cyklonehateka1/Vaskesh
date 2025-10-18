#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "../include/jobs.h"
#include "../include/executor.h"

void execute_command(char **args, bool background) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return;
    }

    if (pid == 0) { // child
        if (background)
            signal(SIGINT, SIG_IGN); // ignore Ctrl+C in background
        execvp(args[0], args);
        perror("exec failed");
        exit(1);
    } else {
        if (background) {
            add_job_to_job_list(pid, args[0]);
            printf("Started background job with PID %d\n", pid);
        } else {
            waitpid(pid, NULL, 0);
        }
    }
}

#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include "../include/jobs.h"

Job job_list[MAX_JOBS];
int job_count = 0;

void add_job_to_job_list(pid_t pid, const char *command) {
    if (job_count < MAX_JOBS) {
        job_list[job_count].pid = pid;
        strncpy(job_list[job_count].command_string, command, sizeof(job_list[job_count].command_string) - 1);
        job_list[job_count].command_string[sizeof(job_list[job_count].command_string) - 1] = '\0';
        job_list[job_count].status = "running";
        job_count++;
    } else {
        fprintf(stderr, "Job list is full!\n");
    }
}

void handle_sigchld(int sig) {
    pid_t pid;
    int status;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        for (int i = 0; i < job_count; i++) {
            if (job_list[i].pid == pid) {
                job_list[i].status = "done";
                printf("\n[%d] %s finished\n", pid, job_list[i].command_string);
                fflush(stdout);
                break;
            }
        }
    }
}

void list_jobs(void) {
    printf("Active jobs:\n");
    for (int i = 0; i < job_count; i++) {
        printf("[%d] %s - %s\n", job_list[i].pid, job_list[i].command_string, job_list[i].status);
    }
}

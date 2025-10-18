#ifndef JOBS_H
#define JOBS_H

#include <sys/types.h>

#define MAX_JOBS 100

typedef struct {
    pid_t pid;
    char command_string[64];
    char *status;
} Job;

extern Job job_list[MAX_JOBS];
extern int job_count;

void add_job_to_job_list(pid_t pid, const char *command);
void handle_sigchld(int sig);
void list_jobs(void);

#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "../include/jobs.h"
#include "../include/executor.h"

void execute_command(
    char **args,
    bool background,
    bool *output_redirection,
    bool *input_redirection,
    bool *append_mode,
    char *output_file,
    char *input_file
    ) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return;
    }

    int fd;

    if (pid == 0) { 
        if (background)
            signal(SIGINT, SIG_IGN);
        
        if (output_redirection){
            if (append_mode) {
              fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            }
            else{
                fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        if (input_redirection) {
            fd = open(input_file, O_RDONLY);
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include "executor.h"
#include "jobs.h"

void execute_parsed_input(ParsedInput *parsed) {
    int num_cmds = parsed->num_commands;
    int pipefds[2 * (num_cmds - 1)]; // store all pipe fds

    // Create all required pipes
    for (int i = 0; i < num_cmds - 1; i++) {
        if (pipe(pipefds + i*2) < 0) {
            perror("pipe failed");
            return;
        }
    }

    for (int i = 0; i < num_cmds; i++) {
        Command *cmd = &parsed->commands[i];
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork failed");
            return;
        }

        if (pid == 0) { 
            if (parsed->background)
                signal(SIGINT, SIG_IGN);

            if (cmd->input_redirection) {
                int fd = open(cmd->input_file, O_RDONLY);
                if (fd < 0) { perror("open input file"); exit(1); }
                dup2(fd, STDIN_FILENO);
                close(fd);
            } else if (i > 0) {
                // Read from previous pipe
                dup2(pipefds[(i-1)*2], STDIN_FILENO);
            }

            // Output redirection
            if (cmd->output_redirection) {
                int fd;
                if (cmd->append_mode)
                    fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
                else
                    fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0) { perror("open output file"); exit(1); }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            } else if (i < num_cmds - 1) {
                // Write to next pipe
                dup2(pipefds[i*2 + 1], STDOUT_FILENO);
            }

            // Close all pipe fds in child
            for (int j = 0; j < 2*(num_cmds-1); j++) close(pipefds[j]);

            execvp(cmd->args[0], cmd->args);
            perror("exec failed");
            exit(1);

        } else { // Parent process
            if (parsed->background)
                add_job_to_job_list(pid, cmd->args[0]);
        }
    }

    // Close all pipe fds in parent
    for (int i = 0; i < 2*(num_cmds-1); i++) close(pipefds[i]);

    if (!parsed->background) {
        for (int i = 0; i < num_cmds; i++) wait(NULL);
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#include "../include/jobs.h"
#include "../include/parser.h"
#include "../include/executor.h"

#define INPUT_SIZE 1024

int main(void) {
    char input[INPUT_SIZE];

    // Set up signal handler for background jobs
    signal(SIGCHLD, handle_sigchld);

    while (1) {
        printf("myshell> ");
        fflush(stdout);

        if (fgets(input, INPUT_SIZE, stdin) == NULL) {
            printf("\n");
            break;
        }

        // Remove newline
        input[strcspn(input, "\n")] = '\0';

        // Exit
        if (strcmp(input, "exit") == 0) {
            printf("Goodbye 👋\n");
            break;
        }

        // Parse command
        char *args[64];
        bool background = false;
        parse_input(input, args, &background);

        if (args[0] == NULL) continue;

        execute_command(args, background);
    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

#include "../include/jobs.h"
#include "../include/parser.h"
#include "../include/executor.h"

#define INPUT_SIZE 1024

int main(void) {
    char input[INPUT_SIZE];

    signal(SIGCHLD, handle_sigchld);

    while (1) {
        printf("myshell> ");
        fflush(stdout);

        if (fgets(input, INPUT_SIZE, stdin) == NULL) {
            printf("\n");
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0) {
            printf("Goodbye 👋\n");
            break;
        }

        char *args[64];
        bool output_redirection;
        bool append_mode;
        bool input_redirection;
        char *output_file = NULL;
        char *input_file = NULL;    

        bool background;
        parse_input(input, args, &background, &output_redirection, &input_redirection, &append_mode, &output_file, &input_file);

        if (args[0] == NULL) continue;

        execute_command(args, background, &output_redirection, &input_redirection, &append_mode, output_file, input_file);
    }

    return 0;
}

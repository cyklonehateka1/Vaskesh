#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "parser.h"
#include "executor.h"
#include "jobs.h"

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

        ParsedInput parsed;
        if (!parse_input(input, &parsed)) continue;

        execute_parsed_input(&parsed);
    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "parser.h"
#include "executor.h"
#include "jobs.h"
#include "ai_requests.h"

#define INPUT_SIZE 1024

int main(void) {
    signal(SIGCHLD, handle_sigchld);
    read_history(".myshell_history");

    while (1) {
        char *line = readline("vaskesh> ");
        fflush(stdout);

        if (line == NULL) {
            printf("\n");
            break;
        }

        if (strlen(line) > 0 && line[strlen(line) - 1] == '\n')
            line[strlen(line) - 1] = '\0';

        if (strcmp(line, "") == 0) {
            free(line);
            continue;
        }

        if (strcmp(line, "exit") == 0) {
            printf("Goodbye 👋\n");
            free(line);
            break;
        }

        add_history(line);
        write_history(".myshell_history");

        char *converted_command = query_ollama(line);

        printf("AI -> %s\n", converted_command);
        if (!converted_command) {
            fprintf(stderr, "AI conversion failed. Try again.\n");
            free(line);
            continue;
        }

        if (!is_safe_command(converted_command)) {
            fprintf(stderr, "Unsafe command blocked: %s\n", converted_command);
            free(converted_command);
            free(line);
            continue;
        }

        ParsedInput parsed;
        if (!parse_input(converted_command, &parsed)) {
            fprintf(stderr, "Failed to parse command.\n");
            free(converted_command);
            free(line);
            continue;
        }

        execute_parsed_input(&parsed);

        free(line);
        free(converted_command);
    }

    return 0;
}

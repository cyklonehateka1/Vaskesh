#include <string.h>
#include <stdbool.h>
#include "../include/parser.h"

void parse_input(
    char *input,
    char **args,
    bool *background,
    bool *output_redirection,
    bool *input_redirection,
    bool *append_mode,
    char **output_file,
    char **input_file
) {
    *background = false;
    *output_redirection = false;
    *input_redirection = false;
    *append_mode = false;
    *output_file = NULL;
    *input_file = NULL;

    int len = strlen(input);
    if (len > 0 && input[len - 1] == '&') {
        *background = true;
        input[len - 1] = '\0';
    }

    int i = 0;
    args[i] = strtok(input, " ");
    while (args[i] != NULL && i < 63) {
        if (strcmp(args[i], ">") == 0) {
            *output_redirection = true;
            *output_file = strtok(NULL, " ");
            args[i] = NULL;
            break;
        } else if (strcmp(args[i], ">>") == 0) {
            *append_mode = true;
            *output_redirection = true;
            *output_file = strtok(NULL, " ");
            args[i] = NULL;
            break;
        } else if (strcmp(args[i], "<") == 0) {
            *input_redirection = true;
            *input_file = strtok(NULL, " ");
            args[i] = NULL;
            break;
        }

        i++;
        args[i] = strtok(NULL, " ");
    }
}

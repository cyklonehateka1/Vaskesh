#include <string.h>
#include <stdbool.h>
#include "../include/parser.h"

void parse_input(char *input, char **args, bool *background) {
    *background = false;

    // Check for '&' at end
    int len = strlen(input);
    if (len > 0 && input[len - 1] == '&') {
        *background = true;
        input[len - 1] = '\0';
    }

    // Tokenize input
    int i = 0;
    args[i] = strtok(input, " ");
    while (args[i] != NULL && i < 63) {
        i++;
        args[i] = strtok(NULL, " ");
    }
}

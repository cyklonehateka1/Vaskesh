#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>

void parse_input(char *input,
    char **args,
    bool *background,
    bool *output_redirection,
    bool *input_redirection,
    bool *append_mode,
    char **output_file,
    char **input_file);

#endif

#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <stdbool.h>

void execute_command(
    char **args,
    bool background,
    bool *output_redirection,
    bool *input_redirection,
    bool *append_mode,
    char *output_file,
    char *input_file);

#endif

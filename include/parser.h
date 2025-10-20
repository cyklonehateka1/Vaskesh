#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>

#define MAX_ARGS 64
#define MAX_CMDS 16

typedef struct {
    char *args[MAX_ARGS];
    bool input_redirection;
    bool output_redirection;
    bool append_mode;
    char *input_file;
    char *output_file;
} Command;

typedef struct {
    Command commands[MAX_CMDS];
    int num_commands;
    bool background;
} ParsedInput;

bool parse_input(char *input, ParsedInput *parsed);

#endif

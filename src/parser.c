#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

bool parse_input(char *input, ParsedInput *parsed) {
    parsed->background = false;
    parsed->num_commands = 0;

    int len = strlen(input);
    if (len == 0) return false;

    // Handle background
    if (input[len - 1] == '&') {
        parsed->background = true;
        input[len - 1] = '\0';
    }

    // Split by pipe
    char *cmd_str = strtok(input, "|");
    while (cmd_str != NULL && parsed->num_commands < MAX_CMDS) {
        Command *cmd = &parsed->commands[parsed->num_commands];
        cmd->input_redirection = false;
        cmd->output_redirection = false;
        cmd->append_mode = false;
        cmd->input_file = NULL;
        cmd->output_file = NULL;

        int arg_index = 0;
        char *token = strtok(cmd_str, " ");
        while (token != NULL && arg_index < MAX_ARGS - 1) {
            if (strcmp(token, "<") == 0) {
                token = strtok(NULL, " ");
                if (!token) { 
                    fprintf(stderr, "Syntax error: input redirection without file\n");
                    return false;
                }
                cmd->input_redirection = true;
                cmd->input_file = token;
            } else if (strcmp(token, ">") == 0) {
                token = strtok(NULL, " ");
                if (!token) { 
                    fprintf(stderr, "Syntax error: output redirection without file\n");
                    return false;
                }
                cmd->output_redirection = true;
                cmd->append_mode = false;
                cmd->output_file = token;
            } else if (strcmp(token, ">>") == 0) {
                token = strtok(NULL, " ");
                if (!token) { 
                    fprintf(stderr, "Syntax error: output append redirection without file\n");
                    return false;
                }
                cmd->output_redirection = true;
                cmd->append_mode = true;
                cmd->output_file = token;
            } else {
                cmd->args[arg_index++] = token;
            }
            token = strtok(NULL, " ");
        }
        cmd->args[arg_index] = NULL;

        parsed->num_commands++;
        cmd_str = strtok(NULL, "|");
    }

    return parsed->num_commands > 0;
}

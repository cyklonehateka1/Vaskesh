#ifndef AI_REQUESTS_H
#define AI_REQUESTS_H

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    char *ptr; 
    size_t len; 
} ai_response_string;

void init_string(ai_response_string *s);
size_t writefunc(void *ptr, size_t size, size_t nmemb, ai_response_string *s);
bool is_safe_command(const char *command);
char* query_ollama(const char *prompt);

#endif

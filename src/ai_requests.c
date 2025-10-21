#include <curl/curl.h>
#include "ai_requests.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void init_string(ai_response_string *s) {
    s->len = 0;
    s->ptr = malloc(1);
    s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, ai_response_string *s) {
    size_t new_len = s->len + size * nmemb;
    s->ptr = realloc(s->ptr, new_len + 1);
    memcpy(s->ptr + s->len, ptr, size * nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;
    return size * nmemb;
}

static void trim_command(char *cmd) {
    if (!cmd) return;

    while (*cmd == ' ' || *cmd == '`' || *cmd == '"' || *cmd == '\'') cmd++;

    memmove(cmd - (cmd - (cmd)), cmd, strlen(cmd) + 1);

    size_t len = strlen(cmd);
    while (len > 0 && (cmd[len - 1] == ' ' || cmd[len - 1] == '`' || cmd[len - 1] == '"' || cmd[len - 1] == '\'')) {
        cmd[--len] = '\0';
    }
}

static char* extract_response_text(const char *json) {
    const char *p = json;
    size_t total_len = 0;
    char *result = malloc(1);
    result[0] = '\0';

    while ((p = strstr(p, "\"response\":\"")) != NULL) {
        p += strlen("\"response\":\"");
        const char *end = strchr(p, '"');
        if (!end) break;

        size_t len = end - p;
        result = realloc(result, total_len + len + 1);
        strncpy(result + total_len, p, len);
        total_len += len;
        result[total_len] = '\0';
        p = end + 1;
    }

    return result;
}

bool is_safe_command(const char *command) {
    return !(strstr(command, "rm") ||
             strstr(command, "shutdown") ||
             strstr(command, "reboot") ||
             strstr(command, "mkfs") ||
             strstr(command, "dd") ||
             strstr(command, ":(){:|:&};:"));
}

char* query_ollama(const char *prompt) {
    CURL *curl = curl_easy_init();
    if (!curl) return NULL;

    ai_response_string s;
    init_string(&s);

    char data[1024];
    snprintf(data, sizeof(data),
        "{\"model\":\"llama3:latest\",\"prompt\":\"Convert this request into a single Linux shell command only: Just the command only, nothing else. Not even you explaining yourself. Example the command 'print working directory' should return pwd only. nothing else %s\"}",
        prompt);

    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:11434/api/generate");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        free(s.ptr);
        return NULL;
    }

    // printf("\n--- RAW OLLAMA RESPONSE START ---\n%s\n--- RAW OLLAMA RESPONSE END ---\n\n", s.ptr);

    char *command = extract_response_text(s.ptr);
    trim_command(command);
    // printf("AI -> %s\n", command ? command : "(null)");

    free(s.ptr);
    return command;
}

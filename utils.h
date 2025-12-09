#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <ctype.h>

#define die(fmt, ...)                                                       \
    do {                                                                    \
        fprintf(stderr, "%s:%d: in %s: error: " fmt "\n",                   \
                __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__);           \
        exit(EXIT_FAILURE);                                                 \
    } while (0)

#define die_errno(fmt, ...)                                                 \
    do {                                                                    \
        fprintf(stderr, "%s:%d: in %s: error: " fmt ": %s(%d)\n",           \
                __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__,            \
                strerror(errno), errno);                                    \
        exit(EXIT_FAILURE);                                                 \
    } while (0)

int run_process(char *argv[]);
int file_exists(char *fn);

static inline void str_to_lower(char *s)
{
    while (*s) {
        *s = tolower(*s);
        s++;
    }
}

static inline void str_replace_chr(char *s, char from, char to)
{
    while (*s) {
        if (*s == from)
            *s = to;
        s++;
    }
}

extern char *EOL;

#endif

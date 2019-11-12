#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "utils.h"

int nala_min_int(int a, int b)
{
    return a < b ? a : b;
}

size_t nala_min_size_t(size_t a, size_t b)
{
    return a < b ? a : b;
}

size_t nala_count_chars(const char *string, char chr)
{
    size_t count = 0;

    for (size_t i = 0; string[i] != '\0'; i++) {
        if (string[i] == chr) {
            count++;
        }
    }

    return count;
}

const char *nala_next_line(const char *string)
{
    char *next_line = strchr(string, '\n');

    if (next_line != NULL) {
        return next_line;
    } else {
        return string + strlen(string);
    }
}

const char *nala_next_lines(const char *string, size_t lines)
{
    const char *next_line = string;

    for (size_t i = 0; i < lines; i++) {
        next_line = nala_next_line(next_line) + 1;
    }

    return next_line;
}

#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

size_t nala_util_read_stream(FILE *stream, char **output_buffer)
{
    char buffer[256];
    size_t output_length = 0;

    size_t read_count = fread(buffer, 1, sizeof(buffer) - 1, stream);
    buffer[read_count] = '\0';

    if (read_count > 0) {
        *output_buffer = malloc(read_count + 1);
        memcpy(*output_buffer, buffer, read_count + 1);
    }

    output_length = read_count;

    while (read_count + 1 == sizeof(buffer)) {
        read_count = fread(buffer, 1, sizeof(buffer) - 1, stream);
        buffer[read_count] = '\0';
        output_length += read_count;

        *output_buffer = realloc(*output_buffer, output_length + 1);
        memcpy(*output_buffer + output_length - read_count, buffer, read_count + 1);
    }

    return output_length;
}

bool nala_is_short_string(const char *string)
{
    return strlen(string) < 64 && strchr(string, '\n') == NULL;
}

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

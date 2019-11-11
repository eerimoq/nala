#ifndef NALA_UTILS_H
#define NALA_UTILS_H

#include <stdbool.h>
#include <stdio.h>

size_t nala_util_read_stream(FILE *stream, char **buffer);
bool nala_is_short_string(const char *string);
int nala_min_int(int a, int b);
size_t nala_min_size_t(size_t a, size_t b);
size_t nala_count_chars(const char *string, char chr);
const char *nala_next_line(const char *string);
const char *nala_next_lines(const char *string, size_t lines);

#endif

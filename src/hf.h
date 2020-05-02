/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Erik Moqvist
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * This file is part of the humanfriendly project.
 */

#include <unistd.h>

#define NALA_HF_VERSION "0.4.0"

/**
 * Get the username of the currently logged in user. Returns the
 * current username, the default username, or NULL if the current user
 * cannot be determined and default_p is NULL.
 */
char *nala_hf_get_username(char *buf_p, size_t size, const char *default_p);

/**
 * Get the hostname. Returns the hostname, the default hostname, or
 * NULL if the hostname cannot be determined and default_p is NULL.
 */
char *nala_hf_get_hostname(char *buf_p, size_t size, const char *default_p);

/**
 * Format given timespan in milliseconds into given buffer.
 */
char *nala_hf_format_timespan(char *buf_p,
                         size_t size,
                         unsigned long long timespan_ms);

/**
 * String to long conversion with limits and default value if out of
 * range or if the string does not contain a number.
 */
long nala_hf_string_to_long(const char *string_p,
                       long minimum,
                       long maximum,
                       long default_value,
                       int base);

char *nala_hf_buffer_to_string(char *dst_p,
                          size_t dst_size,
                          const void *src_p,
                          size_t src_size);

/**
 * Strip leading and trailing characters from a string. The characters
 * to strip are given by `strip_p`.
 */
char *nala_hf_strip(char *str_p, const char *strip_p);

/**
 * Read the whole file. The returned pointer should be freed with
 * free(). Returns NULL on failure.
 */
void *nala_hf_file_read_all(const char *path_p, size_t *size_p);

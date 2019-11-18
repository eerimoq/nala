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

#define NALA_HF_VERSION "0.2.0"

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

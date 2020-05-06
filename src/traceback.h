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
 * This file is part of the traceback project.
 */

#include <stdbool.h>

#define NALA_TRACEBACK_VERSION "0.9.0"

typedef bool (*nala_traceback_skip_filter_t)(void *arg_p, const char *line_p);

/**
 * Format given traceback. buffer_pp and depth are compatible with
 * backtrace() output.
 */
char *nala_traceback_format(void **buffer_pp,
                       int depth,
                       const char *prefix_p,
                       const char *header_p,
                       nala_traceback_skip_filter_t skip_filter,
                       void *arg_p);

/**
 * Create a traceback string.
 */
char *nala_traceback_string(const char *prefix_p,
                       const char *header_p,
                       nala_traceback_skip_filter_t skip_filter,
                       void *arg_p);

/**
 * Print a traceback.
 */
void nala_traceback_print(const char *prefix_p,
                     const char *header_p,
                     nala_traceback_skip_filter_t skip_filter,
                     void *arg_p);

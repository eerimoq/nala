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
 * This file is part of the subprocess project.
 */

#include <string.h>
#include <stdbool.h>

#define NALA_SUBPROCESS_VERSION "0.3.0"

typedef void (*nala_subprocess_entry_t)(void *arg_p);

struct nala_subprocess_output_t {
    /* Always null-terminated. */
    char *buf_p;
    /* Buffer length, not incuding last null-termination. */
    size_t length;
    /* Buffer size, including unused bytes. */
    size_t size;
};

struct nala_subprocess_result_t {
    int exit_code;
    int signal_number;
    struct nala_subprocess_output_t stdout;
    struct nala_subprocess_output_t stderr;
};

/**
 * Call given function with given argument in a subprocess. Returns
 * captured subprocess' exit code, or NULL if the subprocess could not
 * be started.
 */
struct nala_subprocess_result_t *nala_subprocess_call(nala_subprocess_entry_t entry,
                                                      void *arg_p);

/**
 * Call given function with given argument in a subprocess. Returns
 * captured subprocess' stdout, stderr and exit code, or NULL if the
 * subprocess could not be started.
 */
struct nala_subprocess_result_t *nala_subprocess_call_output(nala_subprocess_entry_t entry,
                                                             void *arg_p);

/**
 * Execute given command in a subprocess. Returns captured subprocess'
 * exit code, or NULL if the subprocess could not be started.
 */
struct nala_subprocess_result_t *nala_subprocess_exec(const char *command_p);

/**
 * Execute given command in a subprocess. Returns captured subprocess'
 * stdout, stderr and exit code, or NULL if the subprocess could not
 * be started.
 */
struct nala_subprocess_result_t *nala_subprocess_exec_output(const char *command_p);

/**
 * Returns true if the subprocess was started and exited with 0,
 * otherwise false.
 */
bool nala_subprocess_completed_successfully(struct nala_subprocess_result_t *result_p);

/**
 * Print subprocess exit code, stdout and stderr.
 */
void nala_subprocess_result_print(struct nala_subprocess_result_t *self_p);

/**
 * Free given result.
 */
void nala_subprocess_result_free(struct nala_subprocess_result_t *self_p);

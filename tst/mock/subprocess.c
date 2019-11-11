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

#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "subprocess.h"

static void fatal_error(const char *message_p)
{
    perror(message_p);
    exit(1);
}

static int output_init(struct subprocess_output_t *self_p)
{
    int res;

    res = -1;
    self_p->length = 0;
    self_p->size = 4096;
    self_p->buf_p = malloc(self_p->size);

    if (self_p->buf_p != NULL) {
        self_p->buf_p[0] = '\0';
        res = 0;
    }

    return (res);
}

static void output_append(struct subprocess_output_t *self_p, int fd)
{
    ssize_t res;

    while (1) {
        res = read(fd,
                   &self_p->buf_p[self_p->length],
                   self_p->size - self_p->length - 1);

        if (res == 0) {
            break;
        } else if (res > 0) {
            self_p->length += (size_t)res;
            self_p->buf_p[self_p->length] = '\0';

            if ((self_p->length + 1) == self_p->size) {
                self_p->size += 4096;
                self_p->buf_p = realloc(self_p->buf_p, self_p->size);
            }
        } else {
            if (errno != EINTR) {
                fatal_error("read");
            }
        }
    }
}

static void output_print(struct subprocess_output_t *self_p,
                         const char *name_p)
{
    printf("%s (length: %ld):\n", name_p, self_p->length);
    printf("%s\n", self_p->buf_p);
}

static void redirect_output(int *fds_p, int fileno)
{
    close(fds_p[0]);
    while ((dup2(fds_p[1], fileno) == -1) && (errno == EINTR));
    close(fds_p[1]);
}

static void close_fds(int *fds_p)
{
    close(fds_p[0]);
    close(fds_p[1]);
}

static struct subprocess_result_t *result_new(void)
{
    struct subprocess_result_t *result_p;
    int res;

    result_p = malloc(sizeof(*result_p));

    if (result_p == NULL) {
        return (NULL);
    }

    result_p->exit_code = -1;
    result_p->signal_number = -1;
    res = output_init(&result_p->stdout);

    if (res != 0) {
        goto out1;
    }

    res = output_init(&result_p->stderr);

    if (res != 0) {
        goto out2;
    }

    return (result_p);

 out2:
    free(result_p->stdout.buf_p);

 out1:
    free(result_p);

    return (NULL);
}

static void call_child(subprocess_entry_t entry,
                       void *arg_p)
{
    entry(arg_p);
}

static struct subprocess_result_t *call_parent(pid_t child_pid)
{
    struct subprocess_result_t *result_p;
    int status;

    result_p = result_new();

    waitpid(child_pid, &status, 0);

    if (result_p != NULL) {
        if (WIFEXITED(status)) {
            result_p->exit_code = WEXITSTATUS(status);
        }

        if (WIFSIGNALED(status)) {
            result_p->signal_number = WTERMSIG(status);
        }
    }

    return (result_p);
}

static void call_output_child(subprocess_entry_t entry,
                              void *arg_p,
                              int *stdoutfds_p,
                              int *stderrfds_p)
{
    redirect_output(stdoutfds_p, STDOUT_FILENO);
    redirect_output(stderrfds_p, STDERR_FILENO);
    call_child(entry, arg_p);
}

static struct subprocess_result_t *call_output_parent(pid_t child_pid,
                                                      int *stdoutfds_p,
                                                      int *stderrfds_p)
{
    struct subprocess_result_t *result_p;

    /* Close write ends. */
    close(stdoutfds_p[1]);
    close(stderrfds_p[1]);

    result_p = call_parent(child_pid);

    /* Poll stdout and stderr pipes. */
    if (result_p != NULL) {
        output_append(&result_p->stdout, stdoutfds_p[0]);
        output_append(&result_p->stderr, stderrfds_p[0]);
    }

    close(stdoutfds_p[0]);
    close(stderrfds_p[0]);

    return (result_p);
}

static void exec_entry(const char *command_p)
{
    int res;

    res = execl("/bin/sh", "sh", "-c", command_p, NULL);

    if (res != 0) {
        exit(1);
    }
}

struct subprocess_result_t *subprocess_call(subprocess_entry_t entry,
                                            void *arg_p)
{
    pid_t pid;
    struct subprocess_result_t *result_p;

    fflush(stdout);
    fflush(stderr);

    pid = fork();

    if (pid < 0) {
        result_p = NULL;
    } else if (pid == 0) {
        call_child(entry, arg_p);
        exit(0);
    } else {
        result_p = call_parent(pid);
    }

    return (result_p);
}

struct subprocess_result_t *subprocess_call_output(subprocess_entry_t entry,
                                                   void *arg_p)
{
    pid_t pid;
    int stdoutfds[2];
    int stderrfds[2];
    struct subprocess_result_t *result_p;

    fflush(stdout);
    fflush(stderr);

    if (pipe(stdoutfds) < 0) {
        return (NULL);
    }

    if (pipe(stderrfds) < 0) {
        goto out1;
    }

    pid = fork();

    if (pid < 0) {
        goto out2;
    } else if (pid == 0) {
        call_output_child(entry, arg_p, &stdoutfds[0], &stderrfds[0]);
        exit(0);
    } else {
        result_p = call_output_parent(pid, &stdoutfds[0], &stderrfds[0]);
    }

    return (result_p);

 out2:
    close_fds(&stderrfds[0]);

 out1:
    close_fds(&stdoutfds[0]);

    return (NULL);
}

struct subprocess_result_t *subprocess_exec(const char *command_p)
{
    return (subprocess_call((subprocess_entry_t)exec_entry,
                            (void *)command_p));
}

struct subprocess_result_t *subprocess_exec_output(const char *command_p)
{
    return (subprocess_call_output((subprocess_entry_t)exec_entry,
                                   (void *)command_p));
}

bool subprocess_completed_successfully(struct subprocess_result_t *result_p)
{
    return ((result_p != NULL) && (result_p->exit_code == 0));
}

void subprocess_result_print(struct subprocess_result_t *self_p)
{
    printf("exit_code: %d\n", self_p->exit_code);
    output_print(&self_p->stdout, "stdout");
    output_print(&self_p->stderr, "stderr");
}

void subprocess_result_free(struct subprocess_result_t *self_p)
{
    free(self_p->stdout.buf_p);
    free(self_p->stderr.buf_p);
    free(self_p);
}

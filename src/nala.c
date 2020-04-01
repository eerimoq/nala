#include <libgen.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <ctype.h>
#include <errno.h>
#include <sys/time.h>
#include <getopt.h>
#include "subprocess.h"
#include "traceback.h"
#include "nala.h"
#include "diff/diff.h"
#include "hexdump/hexdump.h"
#include "utils.h"
#include "hf.h"

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"

#define ANSI_BOLD "\x1b[1m"
#define ANSI_RESET "\x1b[0m"

#define COLOR(color, ...) ANSI_RESET ANSI_COLOR_##color __VA_ARGS__ ANSI_RESET

#define BOLD(...) ANSI_RESET ANSI_BOLD __VA_ARGS__ ANSI_RESET

#define COLOR_BOLD(color, ...)                                          \
    ANSI_RESET ANSI_COLOR_##color ANSI_BOLD __VA_ARGS__ ANSI_RESET

struct tests_t {
    struct nala_test_t *head_p;
    struct nala_test_t *tail_p;
};

struct capture_output_t {
    bool running;
    char **output_pp;
    size_t length;
    int original_fd;
    FILE *temporary_file_p;
    FILE *original_file_p;
    FILE *stdout_p;
};

static struct nala_test_t *current_test_p = NULL;

static struct tests_t tests = {
                               .head_p = NULL,
                               .tail_p = NULL
};

static struct capture_output_t capture_stdout;
static struct capture_output_t capture_stderr;

__attribute__ ((weak)) void nala_assert_all_mocks_completed(void)
{
}

__attribute__ ((weak)) void nala_reset_all_mocks(void)
{
}

__attribute__ ((weak)) void nala_suspend_all_mocks(void)
{
}

__attribute__ ((weak)) void nala_resume_all_mocks(void)
{
}

__attribute__ ((weak)) int nala_print_call_mask = 0;

static bool continue_on_failure = false;

static const char *get_node(void)
{
    static char buf[128];

    return (nala_hf_get_hostname(&buf[0], sizeof(buf), "*** unknown ***"));
}

static const char *get_user(void)
{
    static char buf[128];

    return (nala_hf_get_username(&buf[0], sizeof(buf), "*** unknown ***"));
}

static const char *format_timespan(float elapsed_time_ms)
{
    static char buf[128];

    return (nala_hf_format_timespan(&buf[0],
                                    sizeof(buf),
                                    (unsigned long long)elapsed_time_ms));
}

static void color_start(FILE *file_p, const char *color_p)
{
    fprintf(file_p, "%s%s%s", ANSI_RESET, color_p, ANSI_BOLD);
}

static void color_reset(FILE *file_p)
{
    fprintf(file_p, "%s", ANSI_RESET);
}

static char *format_suite_prefix(struct nala_test_t *test_p,
                                 char *buf_p,
                                 size_t size)
{
    size_t length;

    strncpy(buf_p, test_p->file_p, size);
    buf_p = basename(buf_p);
    length = strlen(buf_p);

    if (length < 2) {
        return ("");
    }

    buf_p[length - 2] = '\0';

    if (strcmp(buf_p, "main") == 0) {
        return ("");
    }

    strcat(buf_p, "::");

    return (buf_p);
}

static char *full_test_name(struct nala_test_t *test_p)
{
    static char buf[512];
    char suite[512];
    char *suite_p;

    suite_p = format_suite_prefix(test_p, &suite[0], sizeof(suite));
    snprintf(&buf[0], sizeof(buf), "%s%s", suite_p, test_p->name_p);

    return (&buf[0]);
}

static void capture_output_init(struct capture_output_t *self_p,
                                FILE *file_p)
{
    self_p->output_pp = NULL;
    self_p->length = 0;
    self_p->original_file_p = file_p;
    self_p->stdout_p = fdopen(dup(fileno(file_p)), "w");
}

static void capture_output_destroy(struct capture_output_t *self_p)
{
    if (self_p->output_pp != NULL) {
        if (*self_p->output_pp != NULL) {
            free(*self_p->output_pp);
        }

        self_p->output_pp = NULL;
    }

    if (self_p->stdout_p != NULL) {
        fflush(self_p->stdout_p);
        fclose(self_p->stdout_p);
    }
}

static void capture_output_redirect(struct capture_output_t *self_p)
{
    fflush(self_p->original_file_p);

    self_p->temporary_file_p = tmpfile();
    self_p->original_fd = dup(fileno(self_p->original_file_p));

    while ((dup2(fileno(self_p->temporary_file_p),
                 fileno(self_p->original_file_p)) == -1)
           && (errno == EINTR));
}

static void capture_output_restore(struct capture_output_t *self_p)
{
    fflush(self_p->original_file_p);

    while ((dup2(self_p->original_fd, fileno(self_p->original_file_p)) == -1)
           && (errno == EINTR));

    close(self_p->original_fd);
}

static void capture_output_start(struct capture_output_t *self_p,
                                 char **output_pp)
{
    self_p->output_pp = output_pp;
    self_p->length = 0;
    self_p->running = true;
    capture_output_redirect(self_p);
}

static void capture_output_stop(struct capture_output_t *self_p)
{
    size_t nmembers;

    if (!self_p->running) {
        return;
    }

    self_p->running = false;
    capture_output_restore(self_p);

    self_p->length = (size_t)ftell(self_p->temporary_file_p);
    fseek(self_p->temporary_file_p, 0, SEEK_SET);
    *self_p->output_pp = malloc(self_p->length + 1);

    if (*self_p->output_pp == NULL) {
        printf("Failed to allocate memory.\n");
        exit(1);
    }

    if (self_p->length > 0) {
        nmembers = fread(*self_p->output_pp,
                         self_p->length,
                         1,
                         self_p->temporary_file_p);

        if (nmembers != 1) {
            printf("Failed to read capture output.\n");
            exit(1);
        }
    }

    (*self_p->output_pp)[self_p->length] = '\0';
    fclose(self_p->temporary_file_p);

    printf("%s", *self_p->output_pp);
}

FILE *nala_get_stdout(void)
{
    if (capture_stdout.running) {
        return (capture_stdout.stdout_p);
    } else {
        return (stdout);
    }
}

static float timeval_to_ms(struct timeval *timeval_p)
{
    float res;

    res = (float)timeval_p->tv_usec;
    res /= 1000;
    res += (float)(1000 * timeval_p->tv_sec);

    return (res);
}

static void print_test_failure_report_begin()
{
    printf("-------------------------- "
           "TEST FAILURE REPORT BEGIN "
           "--------------------------\n");
    printf("\n");
}

static void print_test_failure_report_end()
{
    printf("\n");
    printf("--------------------------- "
           "TEST FAILURE REPORT END "
           "---------------------------\n");
}

static void print_signal_failure(struct nala_test_t *test_p)
{
    print_test_failure_report_begin();
    printf("  Test:  " COLOR_BOLD(CYAN, "%s\n"), full_test_name(current_test_p));
    printf("  Error: " COLOR_BOLD(RED, "Terminated by signal %d.\n"),
           test_p->signal_number);
    print_test_failure_report_end();
}

static const char *test_result(struct nala_test_t *test_p, bool color)
{
    const char *result_p;

    if (test_p->executed) {
        if (test_p->exit_code == 0) {
            if (color) {
                result_p = COLOR_BOLD(GREEN, "PASSED");
            } else {
                result_p = "PASSED";
            }
        } else {
            if (color) {
                result_p = COLOR_BOLD(RED, "FAILED");
            } else {
                result_p = "FAILED";
            }
        }
    } else {
        if (color) {
            result_p = COLOR_BOLD(YELLOW, "SKIPPED");
        } else {
            result_p = "SKIPPED";
        }
    }

    return (result_p);
}

static void print_test_result(struct nala_test_t *test_p)
{
    printf("%s %s (" COLOR_BOLD(YELLOW, "%s") ")",
           test_result(test_p, true),
           full_test_name(test_p),
           format_timespan(test_p->elapsed_time_ms));

    if (test_p->signal_number != -1) {
        printf(" (signal: %d)", test_p->signal_number);
    }

    printf("\n");
    fflush(stdout);
}

static void print_summary(struct nala_test_t *test_p,
                          float elapsed_time_ms)
{
    int total;
    int passed;
    int failed;
    int skipped;

    total = 0;
    passed = 0;
    failed = 0;
    skipped = 0;

    while (test_p != NULL) {
        total++;

        if (test_p->executed) {
            if (test_p->exit_code == 0) {
                passed++;
            } else {
                failed++;
            }
        } else {
            skipped++;
        }

        test_p = test_p->next_p;
    }

    printf("\nTests: ");

    if (failed > 0) {
        printf(COLOR_BOLD(RED, "%d failed") ", ", failed);
    }

    if (passed > 0) {
        printf(COLOR_BOLD(GREEN, "%d passed") ", ", passed);
    }

    if (skipped > 0) {
        printf(COLOR_BOLD(YELLOW, "%d skipped") ", ", skipped);
    }

    printf("%d total\n", total);
    printf("Time: " COLOR_BOLD(YELLOW, "%s") "\n",
           format_timespan(elapsed_time_ms));
}

static void write_report_json(struct nala_test_t *test_p)
{
    FILE *file_p;

    file_p = fopen("report.json", "w");

    if (file_p == NULL) {
        exit(1);
    }

    fprintf(file_p,
            "{\n"
            "    \"name\": \"-\",\n"
            "    \"date\": \"-\",\n"
            "    \"node\": \"%s\",\n"
            "    \"user\": \"%s\",\n"
            "    \"testcases\": [\n",
            get_node(),
            get_user());

    while (test_p != NULL) {
        fprintf(file_p,
                "        {\n"
                "            \"name\": \"%s\",\n"
                "            \"description\": [],\n"
                "            \"result\": \"%s\",\n"
                "            \"execution_time\": \"%s\"\n"
                "        }%s\n",
                full_test_name(test_p),
                test_result(test_p, false),
                format_timespan(test_p->elapsed_time_ms),
                (test_p->next_p != NULL ? "," : ""));
        test_p = test_p->next_p;
    }

    fprintf(file_p,
            "    ]\n"
            "}\n");
    fclose(file_p);
}

static void test_entry(void *arg_p)
{
    struct nala_test_t *test_p;

    test_p = (struct nala_test_t *)arg_p;
    capture_output_init(&capture_stdout, stdout);
    capture_output_init(&capture_stderr, stderr);
    nala_reset_all_mocks();
    test_p->func();
    nala_assert_all_mocks_completed();
    nala_reset_all_mocks();
    nala_suspend_all_mocks();
    capture_output_destroy(&capture_stdout);
    capture_output_destroy(&capture_stderr);
    exit(0);
}

static int run_tests(struct nala_test_t *tests_p)
{
    int res;
    struct timeval start_time;
    struct timeval end_time;
    struct timeval test_start_time;
    struct timeval test_end_time;
    struct timeval elapsed_time;
    struct nala_test_t *test_p;
    struct nala_subprocess_result_t *result_p;

    test_p = tests_p;

    while (test_p != NULL) {
        test_p->executed = false;
        test_p = test_p->next_p;
    }

    test_p = tests_p;
    gettimeofday(&start_time, NULL);
    res = 0;

    while (test_p != NULL) {
        gettimeofday(&test_start_time, NULL);
        current_test_p = test_p;
        test_p->before_fork_func();

        result_p = nala_subprocess_call(test_entry, test_p);

        test_p->executed = true;
        test_p->exit_code = result_p->exit_code;
        test_p->signal_number = result_p->signal_number;
        nala_subprocess_result_free(result_p);

        if (test_p->exit_code != 0) {
            res = 1;
        }

        gettimeofday(&test_end_time, NULL);
        timersub(&test_end_time, &test_start_time, &elapsed_time);
        test_p->elapsed_time_ms = timeval_to_ms(&elapsed_time);

        if (test_p->signal_number != -1) {
            print_signal_failure(test_p);
        }

        print_test_result(test_p);

        if ((res != 0) && !continue_on_failure) {
            break;
        }

        test_p = test_p->next_p;
    }

    gettimeofday(&end_time, NULL);
    timersub(&end_time, &start_time, &elapsed_time);
    print_summary(tests_p, timeval_to_ms(&elapsed_time));
    write_report_json(tests_p);

    return (res);
}

bool nala_check_string_equal(const char *actual_p, const char *expected_p)
{
    if (actual_p == expected_p) {
        return (true);
    }

    return ((actual_p != NULL)
            && (expected_p != NULL)
            && (strcmp(actual_p, expected_p) == 0));
}

const char *nala_format(const char *format_p, ...)
{
    va_list vl;
    size_t size;
    char *buf_p;
    FILE *file_p;

    nala_suspend_all_mocks();
    file_p = open_memstream(&buf_p, &size);
    color_start(file_p, ANSI_COLOR_RED);
    va_start(vl, format_p);
    vfprintf(file_p, format_p, vl);
    va_end(vl);
    color_reset(file_p);
    fputc('\0', file_p);
    fclose(file_p);
    nala_resume_all_mocks();

    return (buf_p);
}

static const char *display_inline_diff(FILE *file_p,
                                       const struct NalaDiff *inline_diff,
                                       size_t lines,
                                       const char *string,
                                       size_t *line_number,
                                       bool use_original)
{
    struct NalaDiffChunk *inline_chunk = &inline_diff->chunks[0];
    size_t line_index = 0;
    size_t index = 0;

    for (size_t i = 0; i < lines; i++) {
        const char *next = nala_next_line(string);
        size_t line_length = (size_t)(next - string);

        char line_prefix[64];

        if (use_original) {
            snprintf(line_prefix,
                     sizeof(line_prefix),
                     COLOR(RED, "- ") COLOR_BOLD(RED, "%ld"),
                     *line_number);
            fprintf(file_p, " %37s" COLOR(RED, " |  "), line_prefix);
        } else {
            snprintf(line_prefix,
                     sizeof(line_prefix),
                     COLOR(GREEN, "+ ") COLOR_BOLD(GREEN, "%ld"),
                     *line_number);
            fprintf(file_p, " %37s" COLOR(GREEN, " |  "), line_prefix);
        }

        while (index - line_index < line_length) {
            size_t chunk_end =
                use_original ? inline_chunk->original_end : inline_chunk->modified_end;

            size_t start = index - line_index;
            size_t end = nala_min_size_t(chunk_end - line_index, line_length);

            size_t characters = end - start;

            if (inline_chunk->type == NALA_DIFF_CHUNK_TYPE_MATCHED) {
                fprintf(file_p, "%.*s", (int)characters, string + index - line_index);
            } else if (characters > 0) {
                if (use_original) {
                    fprintf(file_p,
                            COLOR_BOLD(RED, "%.*s"),
                            (int)characters,
                            string + index - line_index);
                } else {
                    fprintf(file_p,
                            COLOR_BOLD(GREEN, "%.*s"),
                            (int)characters,
                            string + index - line_index);
                }
            }

            index += characters;

            if (index >= chunk_end) {
                inline_chunk++;
            }
        }

        fprintf(file_p, "\n");

        if (!use_original) {
            (*line_number)++;
        }

        string = next + 1;
        line_index += line_length + 1;
        index = line_index;
    }

    return string;
}

static void print_string_diff(FILE *file_p,
                              const char *original,
                              const char *modified)
{
    nala_suspend_all_mocks();
    fprintf(file_p, "  Diff:\n\n");

    struct NalaDiff diff = nala_diff_lines(original, modified);

    size_t line_number = 1;

    for (size_t chunk_index = 0; chunk_index < diff.size; chunk_index++) {
        struct NalaDiffChunk *chunk = &diff.chunks[chunk_index];

        size_t original_lines = chunk->original_end - chunk->original_start;
        size_t modified_lines = chunk->modified_end - chunk->modified_start;

        if (chunk->type == NALA_DIFF_CHUNK_TYPE_MATCHED) {
            for (size_t i = 0; i < original_lines; i++) {
                const char *original_next = nala_next_line(original);
                const char *modified_next = nala_next_line(modified);

                if (original_lines < 7 || (i < 2 && chunk_index > 0) ||
                    (original_lines - i < 3 && chunk_index < diff.size - 1)) {
                    fprintf(file_p, COLOR(MAGENTA, "%8zu"), line_number);
                    fprintf(file_p, " |  %.*s\n", (int)(original_next - original), original);
                } else if (i == 2) {
                    fprintf(file_p, "   :\n");
                }

                line_number++;
                original = original_next + 1;
                modified = modified_next + 1;
            }
        } else if (chunk->type == NALA_DIFF_CHUNK_TYPE_REPLACED) {
            const char *original_end = nala_next_lines(original, original_lines);
            const char *modified_end = nala_next_lines(modified, modified_lines);

            size_t original_length = (size_t)(original_end - original);
            size_t modified_length = (size_t)(modified_end - modified);

            struct NalaDiff inline_diff =
                nala_diff_strings_lengths(original,
                                          original_length,
                                          modified,
                                          modified_length);

            original = display_inline_diff(file_p,
                                           &inline_diff,
                                           original_lines,
                                           original,
                                           &line_number,
                                           true);
            modified = display_inline_diff(file_p,
                                           &inline_diff,
                                           modified_lines,
                                           modified,
                                           &line_number,
                                           false);

            free(inline_diff.chunks);
        } else if (chunk->type == NALA_DIFF_CHUNK_TYPE_DELETED) {
            for (size_t i = 0; i < original_lines; i++) {
                const char *original_next = nala_next_line(original);

                char line_prefix[64];
                snprintf(line_prefix,
                         sizeof(line_prefix),
                         COLOR(RED, "- ") COLOR_BOLD(RED, "%ld"),
                         line_number);

                fprintf(file_p, " %37s", line_prefix);
                fprintf(file_p, COLOR(RED, " |  ") COLOR_BOLD(RED, "%.*s\n"),
                        (int)(original_next - original),
                        original);

                original = original_next + 1;
            }
        } else if (chunk->type == NALA_DIFF_CHUNK_TYPE_ADDED) {
            for (size_t i = 0; i < modified_lines; i++) {
                const char *modified_next = nala_next_line(modified);

                char line_prefix[64];
                snprintf(line_prefix,
                         sizeof(line_prefix),
                         COLOR(GREEN, "+ ") COLOR_BOLD(GREEN, "%ld"),
                         line_number);

                fprintf(file_p, " %37s", line_prefix);
                fprintf(file_p, COLOR(GREEN, " |  ") COLOR_BOLD(GREEN, "%.*s\n"),
                        (int)(modified_next - modified),
                        modified);

                line_number++;
                modified = modified_next + 1;
            }
        }
    }

    free(diff.chunks);
    fprintf(file_p, "\n");
    nala_resume_all_mocks();
}

const char *nala_format_string(const char *format_p, ...)
{
    size_t size;
    char *buf_p;
    FILE *file_p;
    va_list vl;
    const char *left_p;
    const char *right_p;

    nala_suspend_all_mocks();

    va_start(vl, format_p);
    left_p = va_arg(vl, const char *);
    right_p = va_arg(vl, const char *);
    va_end(vl);

    file_p = open_memstream(&buf_p, &size);
    color_start(file_p, ANSI_COLOR_RED);
    fprintf(file_p, format_p, left_p, right_p);
    fprintf(file_p, " See diff for details.\n");
    color_reset(file_p);

    if (right_p == NULL) {
        right_p = "<null>";
    }

    if (left_p == NULL) {
        left_p = "<null>";
    }

    print_string_diff(file_p, right_p, left_p);
    fputc('\0', file_p);
    fclose(file_p);

    nala_resume_all_mocks();

    return (buf_p);
}

static void print_with_line_prefix(FILE *file_p,
                                   const char *prefix_p,
                                   const char *string_p)
{
    fprintf(file_p, "%s", prefix_p);

    while (*string_p != '\0') {
        fputc(string_p[0], file_p);

        if (*string_p == '\n') {
            fprintf(file_p, "%s", prefix_p);
        }

        string_p++;
    }
}

const char *nala_format_substring(const char *format_p,
                                  const char *haystack_p,
                                  const char *needle_p)
{
    size_t size;
    char *buf_p;
    FILE *file_p;

    nala_suspend_all_mocks();

    file_p = open_memstream(&buf_p, &size);
    color_start(file_p, ANSI_COLOR_RED);
    fprintf(file_p, "%s", format_p);
    fprintf(file_p, " See below for details.\n");
    color_reset(file_p);

    if (haystack_p == NULL) {
        haystack_p = "<null>";
    }

    if (needle_p == NULL) {
        needle_p = "<null>";
    }

    fprintf(file_p, "  Haystack:\n\n");
    print_with_line_prefix(file_p, "    ", haystack_p);
    fprintf(file_p, "\n");
    fprintf(file_p, "  Needle:\n\n");
    print_with_line_prefix(file_p, "    ", needle_p);
    fprintf(file_p, "\n");
    fputc('\0', file_p);
    fclose(file_p);

    nala_resume_all_mocks();

    return (buf_p);
}

const char *nala_format_memory(const char *prefix_p,
                               const void *left_p,
                               const void *right_p,
                               size_t size)
{
    size_t file_size;
    char *buf_p;
    FILE *file_p;
    char *left_hexdump_p;
    char *right_hexdump_p;

    nala_suspend_all_mocks();

    file_p = open_memstream(&buf_p, &file_size);
    fprintf(file_p,
            COLOR_BOLD(RED, "%sMemory mismatch. See diff for details.\n"),
            prefix_p);

    if (left_p == NULL) {
        left_p = "<null>";
    }

    if (right_p == NULL) {
        right_p = "<null>";
    }

    left_hexdump_p = nala_hexdump(left_p, size, 16);
    right_hexdump_p = nala_hexdump(right_p, size, 16);
    print_string_diff(file_p, right_hexdump_p, left_hexdump_p);
    free(left_hexdump_p);
    free(right_hexdump_p);
    fputc('\0', file_p);
    fclose(file_p);

    nala_resume_all_mocks();

    return (buf_p);
}

bool nala_check_substring(const char *string_p, const char *substring_p)
{
    return ((string_p != NULL)
            && (substring_p != NULL)
            && (strstr(string_p, substring_p) != NULL));
}

bool nala_check_memory(const void *left_p, const void *right_p, size_t size)
{
    return !(((left_p == NULL) && (right_p != NULL))
             || ((left_p != NULL) && (right_p == NULL))
             || (memcmp(left_p, right_p, size) != 0));
}

static bool traceback_skip_filter(void *arg_p, const char *line_p)
{
    (void)arg_p;

    if (strstr(line_p, "nala.c:") != NULL) {
        return (true);
    }

    if (strstr(line_p, "??") != NULL) {
        return (true);
    }

    return (false);
}

void nala_test_failure(const char *message_p)
{
    nala_suspend_all_mocks();
    nala_capture_output_stop();
    capture_output_destroy(&capture_stdout);
    capture_output_destroy(&capture_stderr);
    print_test_failure_report_begin();
    printf("  Test:  " COLOR_BOLD(CYAN, "%s\n"), full_test_name(current_test_p));
    printf("  Error: %s", message_p);
    printf("\n");
    nala_traceback_print("  ", traceback_skip_filter, NULL);
    print_test_failure_report_end();
    free((void *)message_p);
    exit(1);
}

void nala_capture_output_start(char **output_pp, char **errput_pp)
{
    nala_suspend_all_mocks();
    capture_output_start(&capture_stdout, output_pp);
    capture_output_start(&capture_stderr, errput_pp);
    nala_resume_all_mocks();
}

void nala_capture_output_stop()
{
    nala_suspend_all_mocks();
    capture_output_stop(&capture_stdout);
    capture_output_stop(&capture_stderr);
    nala_resume_all_mocks();
}

void nala_register_test(struct nala_test_t *test_p)
{
    if (tests.head_p == NULL) {
        tests.head_p = test_p;
    } else {
        tests.tail_p->next_p = test_p;
    }

    tests.tail_p = test_p;
}

int nala_run_tests()
{
    return (run_tests(tests.head_p));
}

static void print_usage_and_exit(const char *program_name_p, int exit_code)
{
    printf("usage: %s [-h] [-v] [-c] [-a] [<test-pattern>]\n"
           "\n"
           "Run tests.\n"
           "\n"
           "positional arguments:\n"
           "  test-pattern                  Only run tests containing given "
           "pattern.\n"
           "\n"
           "optional arguments:\n"
           "  -h, --help                    Show this help message and exit.\n"
           "  -v, --version                 Print version information.\n"
           "  -c, --continue-on-failure     Always run all tests.\n"
           "  -a, --print-all-calls         Print all calls to ease debugging.\n",
           program_name_p);
    exit(exit_code);
}

static void print_version_and_exit()
{
    printf("%s\n", NALA_VERSION);
    exit(0);
}

static void filter_tests(const char *test_pattern_p)
{
    struct nala_test_t *test_p;

    test_p = tests.head_p;
    tests.head_p = NULL;
    tests.tail_p = NULL;

    while (test_p != NULL) {
        if (strstr(full_test_name(test_p), test_pattern_p) != NULL) {
            nala_register_test(test_p);
        }

        test_p = test_p->next_p;
    }

    if (tests.tail_p != NULL) {
        tests.tail_p->next_p = NULL;
    }
}

__attribute__((weak)) int main(int argc, char *argv[])
{
    static struct option long_options[] = {
        { "help",                no_argument, NULL, 'h' },
        { "version",             no_argument, NULL, 'v' },
        { "continue-on-failure", no_argument, NULL, 'c' },
        { "print-all-calls",     no_argument, NULL, 'a' },
        { NULL,                  no_argument, NULL, 0 }
    };
    int option;

    /* Do not print function calls outside tests. */
    nala_suspend_all_mocks();

    while (1) {
        option = getopt_long(argc, argv, "hvca", &long_options[0], NULL);

        if (option == -1) {
            break;
        }

        switch (option) {

        case 'h':
            print_usage_and_exit(argv[0], 0);
            break;

        case 'v':
            print_version_and_exit();
            break;

        case 'c':
            continue_on_failure = true;
            break;

        case 'a':
            nala_print_call_mask = 0xff;
            break;

        default:
            print_usage_and_exit(argv[0], 1);
        }
    }

    if (optind < argc) {
        filter_tests(argv[optind]);
    }

    return (nala_run_tests());
}

static bool mock_traceback_skip_filter(void *arg_p, const char *line_p)
{
    (void)arg_p;

    if (strstr(line_p, "nala.c:") != NULL) {
        return (true);
    }

    if (strstr(line_p, "nala_mocks.c:") != NULL) {
        return (true);
    }

    if (strstr(line_p, "??") != NULL) {
        return (true);
    }

    return (false);
}

char *nala_mock_traceback_format(void **buffer_pp, int depth)
{
    return (nala_traceback_format(buffer_pp,
                                  depth,
                                  "  ",
                                  mock_traceback_skip_filter,
                                  NULL));
}

#define CHECK_EQ(actual, expected) ((actual) == (expected))

#define CHECK_NE(actual, expected) ((actual) != (expected))

#define CHECK_LT(actual, expected) ((actual) < (expected))

#define CHECK_LE(actual, expected) ((actual) <= (expected))

#define CHECK_GT(actual, expected) ((actual) > (expected))

#define CHECK_GE(actual, expected) ((actual) >= (expected))

#define PRINT_FORMAT(value)                     \
    _Generic((value),                           \
             char: "%c",                        \
             signed char: "%hhd",               \
             unsigned char: "%hhu",             \
             signed short: "%hd",               \
             unsigned short: "%hu",             \
             signed int: "%d",                  \
             unsigned int: "%u",                \
             long int: "%ld",                   \
             unsigned long int: "%lu",          \
             long long int: "%lld",             \
             unsigned long long int: "%llu",    \
             float: "%f",                       \
             double: "%f",                      \
             long double: "%Lf",                \
             const char *: "\"%s\"",            \
             bool: "%d",                        \
             const void *: "%p")

#define PRINT_FORMAT_HEX(value)                 \
    _Generic((value),                           \
             signed char: "%hhx",               \
             unsigned char: "%hhx",             \
             signed short: "%hx",               \
             unsigned short: "%hx",             \
             signed int: "%x",                  \
             unsigned int: "%x",                \
             long int: "%lx",                   \
             unsigned long int: "%lx",          \
             long long int: "%llx",             \
             unsigned long long int: "%llx")

#define ASSERTION(actual, expected, check, format, formatter)   \
    do {                                                        \
        if (!check(actual, expected)) {                         \
            nala_reset_all_mocks();                             \
            char _nala_assert_format[512];                      \
                                                                \
            snprintf(&_nala_assert_format[0],                   \
                     sizeof(_nala_assert_format),               \
                     format,                                    \
                     PRINT_FORMAT(actual),                      \
                     PRINT_FORMAT(expected));                   \
            nala_test_failure(formatter(_nala_assert_format,    \
                                        actual,                 \
                                        expected));             \
        }                                                       \
    } while (0);

#define ASSERTION_WITH_HEX(actual, expected, check, format, formatter)  \
    do {                                                                \
        if (!check(actual, expected)) {                                 \
            nala_reset_all_mocks();                                     \
            char _nala_assert_format[512];                              \
                                                                        \
            snprintf(&_nala_assert_format[0],                           \
                     sizeof(_nala_assert_format),                       \
                     format,                                            \
                     PRINT_FORMAT(actual),                              \
                     PRINT_FORMAT(expected),                            \
                     PRINT_FORMAT_HEX(actual),                          \
                     PRINT_FORMAT_HEX(expected));                       \
            nala_test_failure(formatter(_nala_assert_format,            \
                                        actual,                         \
                                        expected,                       \
                                        actual,                         \
                                        expected));                     \
        }                                                               \
    } while (0);

#define BINARY_ASSERTION(actual, expected, op)                          \
    switch (op) {                                                       \
                                                                        \
    case NALA_CHECK_EQ:                                                 \
        ASSERTION(actual, expected, CHECK_EQ, "%s != %s\n", nala_format); \
        break;                                                          \
                                                                        \
    case NALA_CHECK_NE:                                                 \
        ASSERTION(actual, expected, CHECK_NE, "%s == %s\n", nala_format); \
        break;                                                          \
                                                                        \
    case NALA_CHECK_LT:                                                 \
        ASSERTION(actual, expected, CHECK_LT, "%s >= %s\n", nala_format); \
        break;                                                          \
                                                                        \
    case NALA_CHECK_LE:                                                 \
        ASSERTION(actual, expected, CHECK_LE, "%s > %s\n", nala_format); \
        break;                                                          \
                                                                        \
    case NALA_CHECK_GT:                                                 \
        ASSERTION(actual, expected, CHECK_GT, "%s <= %s\n", nala_format); \
        break;                                                          \
                                                                        \
    case NALA_CHECK_GE:                                                 \
        ASSERTION(actual, expected, CHECK_GE, "%s < %s\n", nala_format); \
        break;                                                          \
                                                                        \
    default:                                                            \
        FAIL("Internal nala error.");                                   \
        break;                                                          \
    }

#define BINARY_ASSERTION_WITH_HEX(actual, expected, op)                 \
    switch (op) {                                                       \
                                                                        \
    case NALA_CHECK_EQ:                                                 \
        ASSERTION_WITH_HEX(actual,                                      \
                           expected,                                    \
                           CHECK_EQ,                                    \
                           "%s != %s (0x%s != 0x%s)\n",                 \
                           nala_format);                                \
        break;                                                          \
                                                                        \
    case NALA_CHECK_NE:                                                 \
        ASSERTION_WITH_HEX(actual,                                      \
                           expected,                                    \
                           CHECK_NE,                                    \
                           "%s == %s (0x%s == 0x%s)\n",                 \
                           nala_format);                                \
        break;                                                          \
                                                                        \
    default:                                                            \
        BINARY_ASSERTION(actual, expected, op);                         \
        break;                                                          \
    }

void nala_assert_char(char actual, char expected, int op)
{
    BINARY_ASSERTION(actual, expected, op);
}

void nala_assert_schar(signed char actual, signed char expected, int op)
{
    BINARY_ASSERTION(actual, expected, op);
}

void nala_assert_uchar(unsigned char actual, unsigned char expected, int op)
{
    BINARY_ASSERTION(actual, expected, op);
}

void nala_assert_short(short actual, short expected, int op)
{
    BINARY_ASSERTION_WITH_HEX(actual, expected, op);
}

void nala_assert_ushort(unsigned short actual, unsigned short expected, int op)
{
    BINARY_ASSERTION_WITH_HEX(actual, expected, op);
}

void nala_assert_int(int actual, int expected, int op)
{
    BINARY_ASSERTION_WITH_HEX(actual, expected, op);
}

void nala_assert_uint(unsigned int actual, unsigned int expected, int op)
{
    BINARY_ASSERTION_WITH_HEX(actual, expected, op);
}

void nala_assert_long(long actual, long expected, int op)
{
    BINARY_ASSERTION_WITH_HEX(actual, expected, op);
}

void nala_assert_ulong(unsigned long actual, unsigned long expected, int op)
{
    BINARY_ASSERTION_WITH_HEX(actual, expected, op);
}

void nala_assert_llong(long long actual, long long expected, int op)
{
    BINARY_ASSERTION_WITH_HEX(actual, expected, op);
}

void nala_assert_ullong(unsigned long long actual,
                        unsigned long long expected,
                        int op)
{
    BINARY_ASSERTION_WITH_HEX(actual, expected, op);
}

void nala_assert_float(float actual, float expected, int op)
{
    BINARY_ASSERTION(actual, expected, op);
}

void nala_assert_double(double actual, double expected, int op)
{
    BINARY_ASSERTION(actual, expected, op);
}

void nala_assert_ldouble(long double actual, long double expected, int op)
{
    BINARY_ASSERTION(actual, expected, op);
}

void nala_assert_bool(bool actual, bool expected, int op)
{
    BINARY_ASSERTION(actual, expected, op);
}

void nala_assert_ptr(const void *actual_p, const void *expected_p, int op)
{
    BINARY_ASSERTION(actual_p, expected_p, op);
}

typedef void (*format_array_item_t)(FILE *file_p, const void *value_p);

static char *format_array(const void *buf_p,
                          size_t item_size,
                          size_t size,
                          int i,
                          format_array_item_t format_item)
{
    size_t file_size;
    char *string_p;
    FILE *file_p;
    int length;
    int begin;
    int end;
    const char *c_buf_p;
    const char *delim_p;

    c_buf_p = (const char *)buf_p;
    length = (int)(size / item_size);
    begin = (i - 3);

    if (begin < 0) {
        begin = 0;
    }

    end = (i + 4);

    if (end > length) {
        end = length;
    }

    file_p = open_memstream(&string_p, &file_size);
    fprintf(file_p, "{ ");

    if (begin != 0) {
        fprintf(file_p, "..., ");
    }

    delim_p = "";

    for (i = begin; i < end; i++) {
        fprintf(file_p, "%s", delim_p);
        format_item(file_p, &c_buf_p[i * (int)item_size]);
        delim_p = ", ";
    }

    if (end != length) {
        fprintf(file_p, ", ...");
    }

    fprintf(file_p, " }");
    fputc('\0', file_p);
    fclose(file_p);

    return (string_p);
}

static void assert_array_failure(const void *actual_p,
                                 const void *expected_p,
                                 size_t item_size,
                                 size_t size,
                                 int i,
                                 format_array_item_t format_item)
{
    size_t file_size;
    char *buf_p;
    FILE *file_p;
    char *actual_string_p;
    char *expected_string_p;

    nala_suspend_all_mocks();

    file_p = open_memstream(&buf_p, &file_size);
    fprintf(file_p,
            COLOR_BOLD(RED, "The arrays differ at index %u. See diff for details.\n"),
            (unsigned)i);
    actual_string_p = format_array(actual_p,
                                   item_size,
                                   size,
                                   i,
                                   format_item);
    expected_string_p = format_array(expected_p,
                                     item_size,
                                     size,
                                     i,
                                     format_item);
    print_string_diff(file_p, expected_string_p, actual_string_p);
    free(actual_string_p);
    free(expected_string_p);
    fputc('\0', file_p);
    fclose(file_p);

    nala_resume_all_mocks();

    nala_test_failure(buf_p);
}

static void format_array_item_char(FILE *file_p, const void *value_p)
{
    fprintf(file_p, "%hhd", (int)*(char *)value_p);
}

static void format_array_item_schar(FILE *file_p, const void *value_p)
{
    fprintf(file_p, "%hhd", *(signed char *)value_p);
}

static void format_array_item_uchar(FILE *file_p, const void *value_p)
{
    fprintf(file_p, "%hhu", *(unsigned char *)value_p);
}

static void format_array_item_short(FILE *file_p, const void *value_p)
{
    fprintf(file_p, "%hd", *(short *)value_p);
}

static void format_array_item_ushort(FILE *file_p, const void *value_p)
{
    fprintf(file_p, "%hu", *(unsigned short *)value_p);
}

static void format_array_item_int(FILE *file_p, const void *value_p)
{
    fprintf(file_p, "%d", *(int *)value_p);
}

static void format_array_item_uint(FILE *file_p, const void *value_p)
{
    fprintf(file_p, "%u", *(unsigned *)value_p);
}

static void format_array_item_long(FILE *file_p, const void *value_p)
{
    fprintf(file_p, "%ld", *(long *)value_p);
}

static void format_array_item_ulong(FILE *file_p, const void *value_p)
{
    fprintf(file_p, "%lu", *(unsigned long *)value_p);
}

static void format_array_item_llong(FILE *file_p, const void *value_p)
{
    fprintf(file_p, "%lld", *(long long *)value_p);
}

static void format_array_item_ullong(FILE *file_p, const void *value_p)
{
    fprintf(file_p, "%llu", *(unsigned long long *)value_p);
}

static void format_array_item_float(FILE *file_p, const void *value_p)
{
    fprintf(file_p, "%f", *(float *)value_p);
}

static void format_array_item_double(FILE *file_p, const void *value_p)
{
    fprintf(file_p, "%f", *(double *)value_p);
}

static void format_array_item_ldouble(FILE *file_p, const void *value_p)
{
    fprintf(file_p, "%Lf", *(long double *)value_p);
}

static void format_array_item_bool(FILE *file_p, const void *value_p)
{
    fprintf(file_p, "%d", *(bool *)value_p);
}

#define ASSERT_ARRAY_TYPE(type, actual_p, expected_p, item_size, size)  \
    size_t i;                                                           \
                                                                        \
    for (i = 0; i < size / item_size; i++) {                            \
        if (actual_p[i] != expected_p[i]) {                             \
            assert_array_failure(actual_p,                              \
                                 expected_p,                            \
                                 item_size,                             \
                                 size,                                  \
                                 (int)i,                                \
                                 format_array_item_ ## type);           \
        }                                                               \
    }                                                                   \

void nala_assert_array_char(const char *actual_p,
                            const char *expected_p,
                            size_t item_size,
                            size_t size)
{
    ASSERT_ARRAY_TYPE(char, actual_p, expected_p, item_size, size);
}

void nala_assert_array_schar(const signed char *actual_p,
                             const signed char *expected_p,
                             size_t item_size,
                             size_t size)
{
    ASSERT_ARRAY_TYPE(schar, actual_p, expected_p, item_size, size);
}

void nala_assert_array_uchar(const unsigned char *actual_p,
                             const unsigned char *expected_p,
                             size_t item_size,
                             size_t size)
{
    ASSERT_ARRAY_TYPE(uchar, actual_p, expected_p, item_size, size);
}

void nala_assert_array_short(const short *actual_p,
                             const short *expected_p,
                             size_t item_size,
                             size_t size)
{
    ASSERT_ARRAY_TYPE(short, actual_p, expected_p, item_size, size);
}

void nala_assert_array_ushort(const unsigned short *actual_p,
                              const unsigned short *expected_p,
                              size_t item_size,
                              size_t size)
{
    ASSERT_ARRAY_TYPE(ushort, actual_p, expected_p, item_size, size);
}

void nala_assert_array_int(const int *actual_p,
                           const int *expected_p,
                           size_t item_size,
                           size_t size)
{
    ASSERT_ARRAY_TYPE(int, actual_p, expected_p, item_size, size);
}

void nala_assert_array_uint(const unsigned int *actual_p,
                            const unsigned int *expected_p,
                            size_t item_size,
                            size_t size)
{
    ASSERT_ARRAY_TYPE(uint, actual_p, expected_p, item_size, size);
}

void nala_assert_array_long(const long *actual_p,
                            const long *expected_p,
                            size_t item_size,
                            size_t size)
{
    ASSERT_ARRAY_TYPE(long, actual_p, expected_p, item_size, size);
}

void nala_assert_array_ulong(const unsigned long *actual_p,
                             const unsigned long *expected_p,
                             size_t item_size,
                             size_t size)
{
    ASSERT_ARRAY_TYPE(ulong, actual_p, expected_p, item_size, size);
}

void nala_assert_array_llong(const long long *actual_p,
                             const long long *expected_p,
                             size_t item_size,
                             size_t size)
{
    ASSERT_ARRAY_TYPE(llong, actual_p, expected_p, item_size, size);
}

void nala_assert_array_ullong(const unsigned long long *actual_p,
                              const unsigned long long *expected_p,
                              size_t item_size,
                              size_t size)
{
    ASSERT_ARRAY_TYPE(ullong, actual_p, expected_p, item_size, size);
}

void nala_assert_array_float(const float *actual_p,
                             const float *expected_p,
                             size_t item_size,
                             size_t size)
{
    ASSERT_ARRAY_TYPE(float, actual_p, expected_p, item_size, size);
}

void nala_assert_array_double(const double *actual_p,
                              const double *expected_p,
                              size_t item_size,
                              size_t size)
{
    ASSERT_ARRAY_TYPE(double, actual_p, expected_p, item_size, size);
}

void nala_assert_array_ldouble(const long double *actual_p,
                               const long double *expected_p,
                               size_t item_size,
                               size_t size)
{
    ASSERT_ARRAY_TYPE(ldouble, actual_p, expected_p, item_size, size);
}

void nala_assert_array_bool(const bool *actual_p,
                            const bool *expected_p,
                            size_t item_size,
                            size_t size)
{
    ASSERT_ARRAY_TYPE(bool, actual_p, expected_p, item_size, size);
}

void nala_assert_array(const void *actual_p,
                       const void *expected_p,
                       size_t item_size,
                       size_t size)
{
    const char *c_actual_p;
    const char *c_expected_p;
    size_t i;
    char buf[512];

    c_actual_p = (const char *)actual_p;
    c_expected_p = (const char *)expected_p;

    for (i = 0; i < size; i += item_size) {
        if (memcmp(&c_actual_p[i], &c_expected_p[i], item_size) != 0) {
            snprintf(&buf[0],
                     sizeof(buf),
                     "The arrays differ at index %u. ",
                     (unsigned)(i / item_size));
            nala_test_failure(nala_format_memory(&buf[0],
                                                 &c_actual_p[i],
                                                 &c_expected_p[i],
                                                 item_size));
        }
    }
}

void nala_assert_string(const char *actual_p, const char *expected_p, int op)
{
    switch (op) {

    case NALA_CHECK_EQ:
        if (!nala_check_string_equal(actual_p, expected_p)) {
            nala_reset_all_mocks();
            nala_test_failure(nala_format_string("The strings are not equal.",
                                                 actual_p,
                                                 expected_p));
        }

        break;

    case NALA_CHECK_NE:
        if (nala_check_string_equal(actual_p, expected_p)) {
            nala_reset_all_mocks();
            nala_test_failure(nala_format("\"%s\" == \"%s\"\n",
                                          actual_p,
                                          expected_p));
        }

        break;

    default:
        FAIL("Internal nala error.");
        break;
    }
}

void nala_assert_substring(const char *haystack_p, const char *needle_p)
{
    if (!nala_check_substring(haystack_p, needle_p)) {
        nala_reset_all_mocks();
        nala_test_failure(
            nala_format_substring(
                "The haystack doesn't contain the needle.",
                haystack_p,
                needle_p));
    }
}

void nala_assert_not_substring(const char *haystack_p, const char *needle_p)
{
    ASSERTION(haystack_p,
              needle_p,
              !nala_check_substring,
              "%s contains %s\n",
              nala_format);
}

void nala_assert_memory(const void *actual_p, const void *expected_p, size_t size)
{
    if (!nala_check_memory(actual_p, expected_p, size)) {
        nala_reset_all_mocks();
        nala_test_failure(nala_format_memory("", actual_p, expected_p, size));
    }
}

void nala_assert(bool cond)
{
    if (!cond) {
        nala_reset_all_mocks();
        nala_test_failure(nala_format("false != true\n"));
    }
}

void nala_fail(const char *message_p)
{
    nala_reset_all_mocks();
    char message[strlen(message_p) + 2];
    strcpy(&message[0], message_p);
    strcat(&message[0], "\n");
    nala_test_failure(nala_format(&message[0]));
}

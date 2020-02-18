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

static bool exit_on_failure = false;

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

    fflush(self_p->stdout_p);
    fclose(self_p->stdout_p);
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
    printf("  Test:  " COLOR(GREEN, "%s\n"), full_test_name(current_test_p));
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

        if ((res != 0) && exit_on_failure) {
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
    fprintf(file_p, "             See diff for details.\n");
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
    printf("  Test:  " COLOR(CYAN, "%s\n"), full_test_name(current_test_p));
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
    printf("usage: %s [-h] [-v] [-e] [-a] [<test-pattern>]\n"
           "\n"
           "Run tests.\n"
           "\n"
           "positional arguments:\n"
           "  test-pattern          Only run tests containing given pattern.\n"
           "\n"
           "optional arguments:\n"
           "  -h, --help                Show this help message and exit.\n"
           "  -v, --version             Print version information.\n"
           "  -e, --exit-on-failure     Exit on first test failure.\n"
           "  -a, --print-all-calls     Print all calls to ease debugging.\n",
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
        { "help",            no_argument, NULL, 'h' },
        { "version",         no_argument, NULL, 'v' },
        { "exit-on-failure", no_argument, NULL, 'e' },
        { "print-all-calls", no_argument, NULL, 'a' },
        { NULL,              no_argument, NULL, 0 }
    };
    int option;

    /* Do not print function calls outside tests. */
    nala_suspend_all_mocks();

    while (1) {
        option = getopt_long(argc, argv, "hva", &long_options[0], NULL);

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

        case 'e':
            exit_on_failure = true;
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

#define CHECK_EQ(actual, expected)                      \
    _Generic(                                           \
        (actual),                                       \
        char *: _Generic(                               \
            (expected),                                 \
            char *: nala_check_string_equal(            \
                (char *)(uintptr_t)(actual),            \
                (char *)(uintptr_t)(expected)),         \
            const char *: nala_check_string_equal(      \
                (char *)(uintptr_t)(actual),            \
                (char *)(uintptr_t)(expected)),         \
            default: false),                            \
        const char *: _Generic(                         \
            (expected),                                 \
            char *: nala_check_string_equal(            \
                (char *)(uintptr_t)(actual),            \
                (char *)(uintptr_t)(expected)),         \
            const char *: nala_check_string_equal(      \
                (char *)(uintptr_t)(actual),            \
                (char *)(uintptr_t)(expected)),         \
            default: false),                            \
        default: (actual) == (expected))

#define CHECK_NE(actual, expected)                              \
    _Generic(                                                   \
        (actual),                                               \
        char *: _Generic(                                       \
            (expected),                                         \
            char *: (!nala_check_string_equal(                  \
                         (char *)(uintptr_t)(actual),           \
                         (char *)(uintptr_t)(expected))),       \
            const char *: (!nala_check_string_equal(            \
                               (char *)(uintptr_t)(actual),     \
                               (char *)(uintptr_t)(expected))), \
            default: true),                                     \
        const char *: _Generic(                                 \
            (expected),                                         \
            char *: (!nala_check_string_equal(                  \
                         (char *)(uintptr_t)(actual),           \
                         (char *)(uintptr_t)(expected))),       \
            const char *: (!nala_check_string_equal(            \
                               (char *)(uintptr_t)(actual),     \
                               (char *)(uintptr_t)(expected))), \
            default: true),                                     \
        default: (actual) != (expected))

#define CHECK_LT(actual, expected) ((actual) < (expected))

#define CHECK_LE(actual, expected) ((actual) <= (expected))

#define CHECK_GT(actual, expected) ((actual) > (expected))

#define CHECK_GE(actual, expected) ((actual) >= (expected))

#define CHECK_SUBSTRING(actual, expected)       \
    nala_check_substring(actual, expected)

#define CHECK_NOT_SUBSTRING(actual, expected)   \
    (!nala_check_substring(actual, expected))

#define FORMAT_EQ(format, actual, expected)                             \
    _Generic(                                                           \
        (actual),                                                       \
        char *: _Generic(                                               \
            (expected),                                                 \
            char *: nala_format_string(                                 \
                format,                                                 \
                (char *)(uintptr_t)(actual),                            \
                (char *)(uintptr_t)(expected)),                         \
            const char *: nala_format_string(                           \
                format,                                                 \
                (char *)(uintptr_t)(actual),                            \
                (char *)(uintptr_t)(expected)),                         \
            default: nala_format(format, (actual), (expected))),        \
        const char *: _Generic(                                         \
            (expected),                                                 \
            char *: nala_format_string(                                 \
                format,                                                 \
                (char *)(uintptr_t)(actual),                            \
                (char *)(uintptr_t)(expected)),                         \
            const char *: nala_format_string(                           \
                format,                                                 \
                (char *)(uintptr_t)(actual),                            \
                (char *)(uintptr_t)(expected)),                         \
            default: nala_format(format, (actual), (expected))),        \
        default: nala_format(format, (actual), (expected)))

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

#define BINARY_ASSERTION(actual, expected, op)                          \
    switch (op) {                                                       \
                                                                        \
    case NALA_CHECK_EQ:                                                 \
        ASSERTION(actual, expected, CHECK_EQ, "%s != %s\n", FORMAT_EQ); \
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
        FAIL();                                                         \
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
    BINARY_ASSERTION(actual, expected, op);
}

void nala_assert_ushort(unsigned short actual, unsigned short expected, int op)
{
    BINARY_ASSERTION(actual, expected, op);
}

void nala_assert_int(int actual, int expected, int op)
{
    BINARY_ASSERTION(actual, expected, op);
}

void nala_assert_uint(unsigned int actual, unsigned int expected, int op)
{
    BINARY_ASSERTION(actual, expected, op);
}

void nala_assert_long(long actual, long expected, int op)
{
    BINARY_ASSERTION(actual, expected, op);
}

void nala_assert_ulong(unsigned long actual, unsigned long expected, int op)
{
    BINARY_ASSERTION(actual, expected, op);
}

void nala_assert_llong(long long actual, long long expected, int op)
{
    BINARY_ASSERTION(actual, expected, op);
}

void nala_assert_ullong(unsigned long long actual,
                        unsigned long long expected,
                        int op)
{
    BINARY_ASSERTION(actual, expected, op);
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

void nala_assert_string(const char *actual_p, const char *expected_p, int op)
{
    BINARY_ASSERTION(actual_p, expected_p, op);
}

void nala_assert_substring(const char *haystack_p, const char *needle_p)
{
    ASSERTION(haystack_p,
              needle_p,
              CHECK_SUBSTRING,
              "%s doesn't contain %s\n",
              nala_format);
}

void nala_assert_not_substring(const char *haystack_p, const char *needle_p)
{
    ASSERTION(haystack_p,
              needle_p,
              CHECK_NOT_SUBSTRING,
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

void nala_fail(void)
{
    nala_reset_all_mocks();
    nala_test_failure(nala_format("fail\n"));
}

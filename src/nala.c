#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <ctype.h>
#include <errno.h>
#include <sys/time.h>
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
};

static struct nala_test_t *current_test_p = NULL;

static struct tests_t tests = {
                               .head_p = NULL,
                               .tail_p = NULL
};

static struct capture_output_t capture_stdout;
static struct capture_output_t capture_stderr;

int setup(void);

int teardown(void);

void nala_assert_all_mocks_completed(void);

__attribute__ ((weak)) void nala_assert_all_mocks_completed(void)
{
}

__attribute__ ((weak)) void nala_reset_all_mocks(void)
{
}

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
    fprintf(file_p, "%s%s%s", ANSI_RESET, ANSI_BOLD, color_p);
}

static void color_reset(FILE *file_p)
{
    fprintf(file_p, "%s", ANSI_RESET);
}

static void capture_output_init(struct capture_output_t *self_p,
                                FILE *file_p)
{
    self_p->output_pp = NULL;
    self_p->length = 0;
    self_p->original_file_p = file_p;
}

static void capture_output_destroy(struct capture_output_t *self_p)
{
    if (self_p->output_pp != NULL) {
        if (*self_p->output_pp != NULL) {
            free(*self_p->output_pp);
        }

        self_p->output_pp = NULL;
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

static float timeval_to_ms(struct timeval *timeval_p)
{
    float res;

    res = (float)timeval_p->tv_usec;
    res /= 1000;
    res += (float)(1000 * timeval_p->tv_sec);

    return (res);
}

static void print_signal_failure(struct nala_test_t *test_p)
{
    printf("\n");
    printf("%s failed:\n\n", test_p->name_p);
    printf("  Location: " COLOR_BOLD(GREEN, "unknown\n"));
    printf("  Error:    " COLOR_BOLD(RED, "Terminated by signal %d.\n"),
           test_p->signal_number);
}

static void print_location_context(const char *filename_p, size_t line_number)
{
    FILE *file_p;
    char line_prefix[64];
    char line[256];
    size_t first_line;
    size_t i;

    printf("  Location context:\n\n");

    file_p = fopen(filename_p, "r");

    if (file_p == NULL) {
        return;
    }

    if (line_number < 2) {
        first_line = 1;
    } else {
        first_line = (line_number - 2);
    }

    for (i = 1; i < line_number + 3; i++) {
        if (fgets(&line[0], sizeof(line), file_p) == NULL) {
            goto out1;
        }

        if (i < first_line) {
            continue;
        }

        if (i == line_number) {
            snprintf(line_prefix,
                     sizeof(line_prefix),
                     "> " COLOR_BOLD(MAGENTA, "%ld"),
                     i);
            printf("  %23s", line_prefix);
            printf(" |  " COLOR_BOLD(CYAN, "%s"), line);
        } else {
            printf("  " COLOR(MAGENTA, "%6zu"), i);
            printf(" |  %s", line);
        }
    }

 out1:

    printf("\n");
    fclose(file_p);
}

static const char *test_result(struct nala_test_t *test_p, bool color)
{
    const char *result_p;

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

    return (result_p);
}

static void print_test_result(struct nala_test_t *test_p)
{
    printf("%s %s (" COLOR_BOLD(YELLOW, "%s") ")",
           test_result(test_p, true),
           test_p->name_p,
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

    total = 0;
    passed = 0;
    failed = 0;

    while (test_p != NULL) {
        total++;

        if (test_p->exit_code == 0) {
            passed++;
        } else {
            failed++;
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
                test_p->name_p,
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
    int res;

    test_p = (struct nala_test_t *)arg_p;

    capture_output_init(&capture_stdout, stdout);
    capture_output_init(&capture_stderr, stderr);

    res = setup();

    if (res == 0) {
        test_p->func();
        res = teardown();

        if (res == 0) {
            nala_assert_all_mocks_completed();
        }
    }

    nala_reset_all_mocks();
    capture_output_destroy(&capture_stdout);
    capture_output_destroy(&capture_stderr);

    exit(res == 0 ? 0 : 1);
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
    gettimeofday(&start_time, NULL);
    res = 0;

    while (test_p != NULL) {
        gettimeofday(&test_start_time, NULL);
        current_test_p = test_p;
        test_p->before_fork_func();

        result_p = nala_subprocess_call(test_entry, test_p);

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
    return (strcmp(actual_p, expected_p) == 0);
}

const char *nala_format(const char *format_p, ...)
{
    va_list vl;
    size_t size;
    char *buf_p;
    FILE *file_p;

    nala_reset_all_mocks();
    file_p = open_memstream(&buf_p, &size);
    color_start(file_p, ANSI_COLOR_RED);
    va_start(vl, format_p);
    vfprintf(file_p, format_p, vl);
    va_end(vl);
    color_reset(file_p);
    fputc('\0', file_p);
    fclose(file_p);

    return (buf_p);
}

static const char *display_inline_diff(FILE *file_p,
                                       const NalaDiff *inline_diff,
                                       size_t lines,
                                       const char *string,
                                       size_t *line_number,
                                       bool use_original)
{
    NalaDiffChunk *inline_chunk = &inline_diff->chunks[0];
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
    fprintf(file_p, "  Diff:\n\n");

    NalaDiff diff = nala_diff_lines(original, modified);

    size_t line_number = 1;

    for (size_t chunk_index = 0; chunk_index < diff.size; chunk_index++) {
        NalaDiffChunk *chunk = &diff.chunks[chunk_index];

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

            NalaDiff inline_diff =
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
}

const char *nala_format_string(const char *format_p, ...)
{
    size_t size;
    char *buf_p;
    FILE *file_p;
    va_list vl;
    const char *left_p;
    const char *right_p;

    va_start(vl, format_p);
    left_p = va_arg(vl, const char *);
    right_p = va_arg(vl, const char *);
    va_end(vl);

    file_p = open_memstream(&buf_p, &size);
    color_start(file_p, ANSI_COLOR_RED);
    fprintf(file_p, format_p, left_p, right_p);
    fprintf(file_p, "            See diff for details.\n");
    color_reset(file_p);
    print_string_diff(file_p, right_p, left_p);
    fputc('\0', file_p);
    fclose(file_p);

    return (buf_p);
}

const char *nala_format_memory(const void *left_p,
                               const void *right_p,
                               size_t size)
{
    size_t file_size;
    char *buf_p;
    FILE *file_p;
    char *left_hexdump_p;
    char *right_hexdump_p;

    file_p = open_memstream(&buf_p, &file_size);
    fprintf(file_p, COLOR_BOLD(RED, "Memory mismatch. See diff for details.\n"));
    left_hexdump_p = nala_hexdump(left_p, size, 16);
    right_hexdump_p = nala_hexdump(right_p, size, 16);
    print_string_diff(file_p, right_hexdump_p, left_hexdump_p);
    free(left_hexdump_p);
    free(right_hexdump_p);
    fputc('\0', file_p);
    fclose(file_p);

    return (buf_p);
}

bool nala_check_substring(const char *actual_p, const char *expected_p)
{
    return ((actual_p != NULL)
            && (expected_p != NULL)
            && (strstr(actual_p, expected_p) != NULL));
}

void nala_test_failure(const char *file_p,
                       int line,
                       const char *message_p)
{
    nala_capture_output_stop();
    capture_output_destroy(&capture_stdout);
    capture_output_destroy(&capture_stderr);
    printf("\n");
    printf("%s failed:\n\n", current_test_p->name_p);
    printf("  Location: " COLOR_BOLD(GREEN, "%s:%d:\n"), file_p, line);
    printf("  Error:    %s", message_p);
    print_location_context(file_p, (size_t)line);
    nala_traceback_print("  ");
    printf("\n");
    exit(1);
}

void nala_capture_output_start(char **output_pp, char **errput_pp)
{
    capture_output_start(&capture_stdout, output_pp);
    capture_output_start(&capture_stderr, errput_pp);
}

void nala_capture_output_stop()
{
    capture_output_stop(&capture_stdout);
    capture_output_stop(&capture_stderr);
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

__attribute__((weak)) int setup(void)
{
    return (0);
}

__attribute__((weak)) int teardown(void)
{
    return (0);
}

__attribute__((weak)) int main(void)
{
    return (nala_run_tests());
}
